// 시스템 전체를 통제하는 클래스
#pragma once
#include "VisionModel.h"
#include "AlarmSystem.h"
#include <opencv2/opencv.hpp>

class SmokeDetectionSystem {
public:
    SmokeDetectionSystem(const std::string& model_path);
    void run(); // 실시간 감지 루프 실행

private:
    VisionModel model;
    AlarmSystem alarm;
    cv::VideoCapture video;
};

