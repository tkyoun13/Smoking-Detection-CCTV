#include "SmokeDetectionSystem.h"

int main(int argc, char* argv[]) {
    // 인자 개수 확인 (모델 경로 필요)
    if (argc != 2) {
        fprintf(stderr, "Usage: ./detector <model_path.tflite>\n");
        return 1;
    }

    try {
        // 시스템 객체 생성 및 실행
        SmokeDetectionSystem system(argv[1]);
        system.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
