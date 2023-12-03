#include <iostream>
#include "CreateFrame.h"

int main()
{
	std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    CreateFrame cFrame;
	cFrame.start();
	std::cout << "started" << std::endl;
}


