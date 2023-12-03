#pragma once
#ifndef YOLO_CPP
#define YOLO_CPP
#include<iostream>
#include<fstream>
#include<vector>
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include "Configuration.h"
#include "CSV_Save.h"
#include "Logging.h"
#include "Sqlite.h"

using namespace cv::dnn;
using namespace std;
using namespace cv;

class Yolo
{
	// Constants.
	//These lines define the input width and height for processing the image in the YOLO
	const float INPUT_WIDTH = 640.0;
	const float INPUT_HEIGHT = 640.0;
	//This line defines the minimum confidence score 
	const float SCORE_THRESHOLD = 0.5;
	//These line defines the threshold for the non-maximum suppression
	const float NMS_THRESHOLD = 0.45;
	const float CONFIDENCE_THRESHOLD = 0.45;

	// Text parameters.
	//This line sets the scale for the font used when displaying text on the image.
	const float FONT_SCALE = 0.7;
	//This line defines the font face used when displaying text.
	const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
	//This line sets the thickness of the lines used when drawing shapes on the image, such as bounding boxes.
	const int THICKNESS = 1;

	// Colors.
	cv::Scalar BLACK = cv::Scalar(0, 0, 0);
	cv::Scalar BLUE = cv::Scalar(255, 178, 50);
	cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
	cv::Scalar RED = cv::Scalar(0, 0, 255);

public:
	Yolo() { }
	~Yolo() { }
	float getINPUT_WIDTH()const;
	float getINPUT_HEIGHT()const;
	float getSCORE_THRESHOLD()const;
	float getNMS_THRESHOLD()const;
	float getCONFIDENCE_THRESHOLD()const;
	float getFONT_SCALE()const;
	int getFONT_FACE()const;
	int getTHICKNESS()const;
	Scalar getBLACK()const;
	Scalar getBLUE()const;
	Scalar getYELLOW()const;
	Scalar getRED()const;
	Mat post_process(cv::Mat& input_image,std:: vector<cv::Mat>& outputs, const std::vector<string>& class_name);
	vector<string> createClass_List();
	Mat createYoloImage(Yolo& image, cv::Mat frame, cv::dnn::Net net);
	void CalcAvg(Mat img, double& avgR, double& avgG, double& avgB);
	void CalcSaveAvg(Mat img, int x, int y, int width, int height);
	void draw_label(Mat& input_image, string label, int left, int top);
	vector<Mat> pre_process(Mat& input_image, Net& net);	
};
#endif


