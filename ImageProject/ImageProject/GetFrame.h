#pragma once
//#ifndef GETFRAME_CPP
//#define GETFRAME_CPP
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "../Service/my_proto/camera.grpc.pb.h"
#include "../Service/my_proto/camera.pb.h"
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include "CreateFrame.h"
#include "FramesQueue.h"
#include "Logging.h"
#include "Yolo.h"

using namespace std;
using namespace cv;

class GetFrame {
private:
	Yolo y_Image;
	cv::Mat frameToRead;
	cv::Mat frameToShow;
	cv::dnn::Net loadNet;
	char key;
public:
	GetFrame();
	~GetFrame() {}
	char getKey();
	void setKey(char value);
	Mat popFrame();
	Mat yoloImage();
	void detection();
	void addFrameToQueue(Mat frame);
};