CC := g++
CFLAGS := -std=c++11 -I/usr/include -I/usr/local/include/opencv4
INCLUDEPATHS := -I${HOME}/tensorflow -I${HOME}/EAI_TfLite
LDFLAGS := -pthread -ltensorflow-lite -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_dnn -lopencv_imgcodecs -lflatbuffers -l:libedgetpu.so.1.0 -lwiringPi -ldl

LDPATH := -L/usr/local/lib \
          -L${HOME}/tensorflow/tensorflow/lite/tools/make/gen/bbb_armv7l/lib \
          -L${HOME}/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/build \
          -L${HOME}/EAI_TfLite/libs/armv7a

SRCS := main.cc VisionModel.cc AlarmSystem.cc SmokeDetectionSystem.cc
OBJS := $(SRCS:.cc=.o)
EXEC := detector

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -o $@ $^ $(LDPATH) $(LDFLAGS)  

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c $< -o $@   

clean : 
	rm -f $(OBJS) $(EXEC)
