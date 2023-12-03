#include "CreateFrame.h"
#define INEQUALITY_PERCENTAGES_TO_DECIPHER 0.25
#ifdef _WIN32
#define PATH_VIDEO Configuration::getConfiguration().getConfigValue(eConfig::windows_videoPath)
#elif __linux__
//#include <unistd.h>  // for sleep()
//#include <termios.h> // for getch()
//#include <curses.h>  // for kbhit()
#include <ncurses.h>
#define PATH_VIDEO Configuration::getConfiguration().getConfigValue(eConfig::linux_videoPath)
#endif

CreateFrame::CreateFrame()
{
	Logging::getFile()->debug("Create Frame constructor");
}

CreateFrame::~CreateFrame()
{
	Logging::getFile()->debug("Create Frame destructor");
}

bool CreateFrame::isFramesDiff(cv::Mat lastFrame, cv::Mat frame) {
	Logging::getFile()->debug("in calc diff between frames");
	cv::Mat diffFrames;
	//Calculate absolute difference between the frames
	absdiff(lastFrame, frame, diffFrames);
	if (diffFrames.channels() > 1)
		cvtColor(diffFrames, diffFrames, cv::COLOR_BGR2GRAY);
	double amountDiffPixels = static_cast<double>(countNonZero(diffFrames > 15));
	double amountPixelsNeeded = frame.size().area() * INEQUALITY_PERCENTAGES_TO_DECIPHER;
	if (amountDiffPixels >= amountPixelsNeeded)
		return true;
	Logging::getFile()->info("This frame is very similar to the previous one - continue to the next one");
	return false;
}
void CreateFrame::convertTogRPC(Mat frame)
{
	Logging::getFile()->debug("i am in convert");
	// Convert the image to bytes
	std::vector<uchar> imageData;
	cv::imencode(".png", frame, imageData);
	//make the frame to send it to server
	image_gRPC.set_channels(frame.channels());
	image_gRPC.set_height(frame.rows);
	image_gRPC.set_width(frame.cols);
	image_gRPC.set_timestampvideo(to_string(cap.get(CAP_PROP_POS_MSEC)));
	image_gRPC.set_imagedata(imageData.data(), imageData.size());
}

auto CreateFrame::sendTheFirstImage() {
	Logging::getFile()->debug("I'm in start");
	cv::VideoCapture cap((string)PATH_VIDEO);
	if (!cap.isOpened())
		Logging::getFile()->error("Error opening video stream or file");
	cap >> frameToWrite;
	frameToWrite.copyTo(lastFrame);
	convertTogRPC(frameToWrite);
	//make a router
	//auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	auto channel = grpc::CreateChannel("85.130.224.75:50051", grpc::InsecureChannelCredentials());
	std::unique_ptr<expcmake::Greeter::Stub> stub = expcmake::Greeter::NewStub(channel);
	grpc::ClientContext context;
	Logging::getFile()->info("send to server");
	//call to server and send a frame
	grpc::Status status = stub->getMat(&context, image_gRPC, &result);
	//get a message
	//Logging::getFile()->info("result:" + result.time());
	cout << "result first:" << result.message() << endl;
	return channel;
}
void CreateFrame::sendFrames(std::shared_ptr<grpc::Channel> channel) {
	cap >> frameToWrite;
	int frameCounter = 1;
	while (!(frameToWrite.empty()) && key != 27)
	{
		Logging::getFile()->debug("the next iteration");
		double capFrameRate = cap.get(CAP_PROP_FPS);
		const int frameSkip = static_cast<int>(capFrameRate / 3) / 3;//The requirement that only 3 frames per second
		if (isFramesDiff(lastFrame, frameToWrite) && frameCounter % frameSkip == 0)
		{
			convertTogRPC(frameToWrite);
			std::unique_ptr<expcmake::Greeter::Stub> stub = expcmake::Greeter::NewStub(channel);
			grpc::ClientContext context;
			//call to server and send a frame
			grpc::Status status = stub->getMat(&context, image_gRPC, &result);
			//get a message
			//Logging::getFile()->info("result:" + result.time());
			cout << "result:" << result.message() << endl;
			frameToWrite.copyTo(lastFrame);
		}

		frameCounter++;

#ifdef __linux__
		sleep(1);
		key = getch();  // Read a character from the keyboard
#else
		Sleep(1);
		if (_kbhit())
			key = _getch();
#endif
		cap >> frameToWrite;
	}

#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
	cap.release();
}
void CreateFrame::start()
{
	std::shared_ptr<grpc::Channel> channel=sendTheFirstImage();
	sendFrames(channel);
}

