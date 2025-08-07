// 영상 감지 + 하드웨어 동작 연동 로직 구현
#include "SmokeDetectionSystem.h"

SmokeDetectionSystem::SmokeDetectionSystem(const std::string& model_path)
    : model(model_path), alarm(29, 6, 1), video(0) {
    if (!video.isOpened()) {
        std::cerr << "Unable to open camera\n";
        exit(1);
    }
}

void SmokeDetectionSystem::run() {
    cv::Mat frame;
    while (video.read(frame)) {
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        if (model.detect(frame, classIds, confidences, boxes)) {
            std::cout << "Detected all labels, activating alarm...\n";
            cv::imshow("Detection", frame);
            cv::waitKey(3000);
            alarm.activate();
            model.labels_detected = std::vector<bool>(3, false);
        }

        for (size_t i = 0; i < boxes.size(); ++i) {
            cv::rectangle(frame, boxes[i], cv::Scalar(0, 0, 255), 2);
            std::string label = "Class: " + std::to_string(classIds[i]) +
                " Conf: " + std::to_string(confidences[i]);
            cv::putText(frame, label, cv::Point(boxes[i].x, boxes[i].y - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        }

        cv::imshow("Detection", frame);
        if (cv::waitKey(1) == 27) break;
    }
    video.release();
    cv::destroyAllWindows();
}

