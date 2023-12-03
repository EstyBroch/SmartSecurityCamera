#pragma once​
#ifndef FRAMESQUEUE_CPP
#define FRAMESQUEUE_CPP
#include <queue>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include <condition_variable>
#include "Logging.h"
#include "Configuration.h"
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#define QUEUE_SIZE Configuration::getConfiguration().getConfigValue(eConfig::queueSize)

using namespace cv::dnn;
using namespace std;
using namespace cv;

class ProtectedFramesQueue{
private:
	queue<Mat> framesQueue;
	mutex mtx;
	condition_variable condition;
	inline static ProtectedFramesQueue* protectedFramesQueue = NULL;
public:
	ProtectedFramesQueue();
	~ProtectedFramesQueue() {}
	ProtectedFramesQueue(ProtectedFramesQueue& other) = delete;
	void operator=(const ProtectedFramesQueue&) = delete;
	void push(Mat mat);
	Mat front();
	Mat pop();
	Mat copyTo(Mat other);
	size_t size();
	bool empty();
	static ProtectedFramesQueue& getFramesQueue();
};
#endif