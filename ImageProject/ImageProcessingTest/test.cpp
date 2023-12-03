#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#include "CSV_Save.cpp"
#include "GetFrame.h"
#include "FramesQueue.h"

using namespace cv::dnn;
using namespace std;

#define TEST_QUEUE_SIZE 100
#ifdef UNIT_TESTING
#define QUEUE_SIZE TEST_QUEUE_SIZE
#endif
#define PATH_CSV "avarage.csv"

TEST_CASE("Sandbox") {
    Mat im(3, 3, CV_8UC3);
    im.at<Vec3b>(0, 0) = Vec3b(10, 123, 4);
    im.at<Vec3b>(0, 1) = Vec3b(10, 123, 4);
    im.at<Vec3b>(0, 2) = Vec3b(10, 123, 4);
    im.at<Vec3b>(1, 0) = Vec3b(10, 123, 4);
    im.at<Vec3b>(1, 1) = Vec3b(10, 123, 4);
    im.at<Vec3b>(1, 2) = Vec3b(10, 123, 4);
    im.at<Vec3b>(2, 0) = Vec3b(10, 123, 4);
    im.at<Vec3b>(2, 1) = Vec3b(10, 123, 4);
    im.at<Vec3b>(2, 2) = Vec3b(10, 123, 4);
    CSV_Save CSV_ImageFile;
    CSV_ImageFile.save(4, 123, 10, 0, 0, 3, 3);
    string expectedLine = "4,123,10,0,0,3,3";
    ifstream file(PATH_CSV, ios::in);
    if (!file.is_open())
        throw "Invalid path";
    string lineAvg;
    string lastLine;
    while (getline(file, lineAvg))
    {
        lastLine = lineAvg; // Store each line in the lastLine variable
    }
    file.close();
    //test the last line at file if it what expected
    REQUIRE(expectedLine == lastLine);
    REQUIRE(true);

    FramesQueue& framesQueue = FramesQueue::getFramesQueue();
}

TEST_CASE("6th_Overrides_The_1st") {
    //test the 6th frame overrides the 1st one.
    cv::Mat firstImg(3, 3, CV_8UC3);
    firstImg.at<cv::Vec3b>(0, 0) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(0, 1) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(0, 2) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(1, 0) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(1, 1) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(1, 2) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(2, 0) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(2, 1) = cv::Vec3b(10, 123, 4);
    firstImg.at<cv::Vec3b>(2, 2) = cv::Vec3b(10, 123, 4);
    cv::Mat img(3, 3, CV_8UC3);
    img.at<cv::Vec3b>(0, 0) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(0, 1) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(0, 2) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(1, 0) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(1, 1) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(1, 2) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(2, 0) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(2, 1) = cv::Vec3b(10, 2, 134);
    img.at<cv::Vec3b>(2, 2) = cv::Vec3b(10, 2, 134);

    //insert the first (this the 6th need override)
    FramesQueue& framesQueue = FramesQueue::getFramesQueue();
    framesQueue.push(firstImg);

    GetFrame gFrame;
    std::vector<std::thread> threads;
    for(size_t i = 0; i < QUEUE_SIZE; i++)
    {
        threads.push_back(std::thread([&gFrame, img]() {
            gFrame.addFrameToQueue(img);
            }));
    }
    /*for (size_t i = 0; i < QUEUE_SIZE; i++)
    {
        threads.push_back(std::thread(&GetFrame::addFrameToQueue, img));
    }*/
    REQUIRE(QUEUE_SIZE == framesQueue.size());

    for (size_t i = 0; i < QUEUE_SIZE; i++)
    {
        threads[i].join();
    }
    cv::Mat diff;
    cv::absdiff(framesQueue.front(), img, diff);
    REQUIRE(cv::countNonZero(diff) == 0);
    //empty the queue
    for (size_t i = 0; i < QUEUE_SIZE; i++)
    {
        framesQueue.pop();
    }
}

TEST_CASE("multiple threads")
{
    FramesQueue& queue = FramesQueue::getFramesQueue();
    std::vector<std::thread> threads;
    // Create 10 threads that push 100 items to the queue
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([&queue]() {
            for (int j = 0; j < 10; ++j)
            {
                Mat img(j, j, CV_8UC3);
                queue.push(img);
            }
            });
    }
    REQUIRE(queue.size() == 100);
    // Create 10 threads that pop 100 items from the queue
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([&queue]()
            {
                for (int j = 0; j < 100; ++j)
                {
                    queue.pop();
                }
            });
    }
    // Wait for all threads to finish
    for (auto& thread : threads)
    {
        thread.join();
    }
    // Test if the queue is empty
    REQUIRE(queue.size() == 0);
}
