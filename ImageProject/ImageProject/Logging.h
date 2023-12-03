#pragma once
#include<iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

class Logging
{
private:
    static std::shared_ptr<spdlog::logger> logger;
public:
    Logging();
    ~Logging() {}
    Logging(Logging& other) = delete;
    void operator=(const Logging&) = delete;
    //static std::shared_ptr<spdlog::logger> getLogFile();
    static std::shared_ptr<spdlog::logger> getFile();
};
