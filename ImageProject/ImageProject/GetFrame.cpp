#include "GetFrame.h"

#ifdef _WIN32
#define PATH_NET Configuration::getConfiguration().getConfigValue(eConfig::windows_netPath)
#elif __linux__
//#include <unistd.h>  // for sleep()
//#include <termios.h> // for getch()
//#include <curses.h>  // for kbhit()
#include <ncurses.h>
#define PATH_NET Configuration::getConfiguration().getConfigValue(eConfig::linux_netPath)
#endif

GetFrame::GetFrame()
{
	loadNet = cv::dnn::readNet(PATH_NET);
	Logging::getFile()->info("readYolo");
}

char GetFrame::getKey()
{
	return key;
}

void GetFrame::setKey(char value)
{
	key = value;
}

Mat GetFrame::popFrame()
{
	return ProtectedFramesQueue::getFramesQueue().front();
}

Mat GetFrame::yoloImage()
{
	Logging::getFile()->info("(..yoloImage..)");
	Mat Y_Image = y_Image.createYoloImage(y_Image, frameToRead, loadNet);
	return Y_Image;
}
void GetFrame::detection()
{
	while (key != 27)
	{
		if (!ProtectedFramesQueue::getFramesQueue().empty())
		{
		   //Pull out the first frame form the queue
		   frameToRead = ProtectedFramesQueue::getFramesQueue().copyTo(frameToRead);
		   Logging::getFile()->info("pop frame from/ queue");
		   //Create yolo image
		   frameToShow = yoloImage();
		   Logging::getFile()->info("my yolo");
		   cout << " " << endl;
		   imshow("Output", frameToShow);
		   waitKey(0);
		}
		#ifdef __linux__
				sleep(1);
				key = getch();  // Read a character from the keyboard
		#else
				Sleep(1);
				if (_kbhit())
					key = _getch();
		#endif
	}
	destroyAllWindows();
	Logging::getFile()->info("pressed on Esc");
}
void GetFrame::addFrameToQueue(cv::Mat frame)
{
	Logging::getFile()->debug("addFrameToQueue");
	try {
		ProtectedFramesQueue::getFramesQueue().push(frame);
		Logging::getFile()->info("push frame to queue");
	}
	catch (const exception& e) {
		// Exception handling code
		Logging::getFile()->error("an exception occurred pushing frame to queue");
		cerr << "An exception occurred: " << e.what() << endl;
	}
}
