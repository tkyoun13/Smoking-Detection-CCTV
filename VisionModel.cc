// 모델 초기화 및 객체 탐지 기능 구현
#include "VisionModel.h"
#include <cassert>

#define TFLITE_MINIMAL_CHECK(x) \
  if (!(x)) { \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1); \
  }

VisionModel::VisionModel(const std::string& model_path) : labels_detected(3, false) {
    // 모델 로드
    auto model = tflite::FlatBufferModel::BuildFromFile(model_path.c_str());
    TFLITE_MINIMAL_CHECK(model != nullptr);

    // 인터프리터 생성
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*model, resolver);
    builder(&interpreter);
    TFLITE_MINIMAL_CHECK(interpreter != nullptr);

    // EdgeTPU delegate 연결
    size_t num_devices;
    std::unique_ptr<edgetpu_device, decltype(&edgetpu_free_devices)> devices(
        edgetpu_list_devices(&num_devices), &edgetpu_free_devices);
    assert(num_devices > 0);
    const auto& device = devices.get()[0];
    auto* delegate = edgetpu_create_delegate(device.type, device.path, nullptr, 0);
    interpreter->ModifyGraphWithDelegate(delegate);

    TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);

    const TfLiteTensor* output_tensor_info = interpreter->tensor(interpreter->outputs()[0]);
    output_scale = output_tensor_info->params.scale;
    output_zero_point = output_tensor_info->params.zero_point;
    output_dims = output_tensor_info->dims;
}

VisionModel::~VisionModel() {}

bool VisionModel::detect(const cv::Mat& frame, std::vector<int>& classIds, std::vector<float>& confidences, std::vector<cv::Rect>& boxes) {
    // 영상 크기 조정 후 텐서 복사
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(input_width, input_height));
    auto input_tensor = interpreter->typed_input_tensor<uint8_t>(0);
    std::vector<uint8_t> input_data(resized.data, resized.data + resized.total() * resized.channels());
    memcpy(input_tensor, input_data.data(), input_data.size());

    // 추론 실행
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    auto output_tensor = interpreter->typed_output_tensor<uint8_t>(0);

    int num_detections = output_dims->data[1];
    for (int i = 0; i < num_detections; ++i) {
        int offset = i * 8;
        float object_conf = (output_tensor[offset + 4] - output_zero_point) * output_scale;

        if (object_conf > 0.5f) {
            float center_x = (output_tensor[offset + 0] - output_zero_point) * output_scale * input_width;
            float center_y = (output_tensor[offset + 1] - output_zero_point) * output_scale * input_height;
            float width = (output_tensor[offset + 0] - output_zero_point) * output_scale * input_width;
            float height = (output_tensor[offset + 1] - output_zero_point) * output_scale * input_height;
            int x = static_cast<int>(center_x - width / 2);
            int y = static_cast<int>(center_y - height / 2);

            int best_class = -1;
            float max_prob = 0;
            for (int j = 0; j < 3; ++j) {
                float class_prob = (output_tensor[offset + 5 + j] - output_zero_point) * output_scale;
                if (class_prob > max_prob) {
                    max_prob = class_prob;
                    best_class = j;
                }
            }

            if (max_prob * object_conf > 0.5f) {
                classIds.push_back(best_class);
                confidences.push_back(max_prob * object_conf);
                boxes.push_back(cv::Rect(x, y, static_cast<int>(width), static_cast<int>(height)));
                if (best_class >= 0 && best_class < 3) {
                    labels_detected[best_class] = true;
                }
            }
        }
    }
    return labels_detected[0] && labels_detected[1] && labels_detected[2];
}