// 딥러닝 모델을 로드하고 영상에서 객체를 탐지하는 클래스
#pragma once
#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include "/home/pi/EAI_TfLite/headers/edgetpu_c.h"
#include <memory>
#include <vector>

class VisionModel {
public:
    VisionModel(const std::string& model_path);  // 모델 경로로 초기화
    ~VisionModel();
    bool detect(const cv::Mat& frame, std::vector<int>& classIds, std::vector<float>& confidences, std::vector<cv::Rect>& boxes); // 객체 감지 수행
    std::vector<bool> labels_detected; // 얼굴, 담배, 연기 탐지 여부 저장

private:
    std::unique_ptr<tflite::Interpreter> interpreter;
    float output_scale;
    int output_zero_point;
    const TfLiteIntArray* output_dims;
    int input_width = 640;
    int input_height = 640;
};

