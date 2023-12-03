#pragma once
#ifndef CREATEFRAME_CPP
#define CREATEFRAME_CPP
#include "FramesQueue.h"
#include "Yolo.h"
#include "../Service/my_proto/camera.grpc.pb.h"
#include "../Service/my_proto/camera.pb.h"
#include <opencv2/dnn.hpp>
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

using namespace cv::dnn;
using namespace cv;

class CreateFrame {

private:
	cv::Mat lastFrame;
	cv::Mat frameToWrite;
	char key;
	cv::VideoCapture cap;
	expcmake::Frame image_gRPC;
	expcmake::CreateMat result;

public:
	CreateFrame();
	~CreateFrame();
	bool isFramesDiff(cv::Mat lastFrame, cv::Mat frame);
	void start();
	void convertTogRPC(Mat frame);
	auto sendTheFirstImage();
	void sendFrames(std::shared_ptr<grpc::Channel>);
};
#endif