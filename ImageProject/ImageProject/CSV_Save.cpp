#define _CRT_SECURE_NO_WARNINGS
#include "CSV_Save.h"

CSV_Save::CSV_Save()
{
	if (isFirstInstance) {
		isFirstInstance = false;
		ofstream dataBase
		((string)SAVE_TO_DB_PATH, ios::app);
		if (!dataBase.is_open())
			throw "Unable to open file.";
		//The Headers of the file
		dataBase << "numFrame,Average R, Average G, Average B,X Left,Y Top,Width,Heigth" << endl;
		Logging::getFile()->info("Written the header");
	}
}

void CSV_Save::save(double avgR, double avgG, double avgB, float x, float y, float width, float height)
{
	ofstream dataBase((string)SAVE_TO_DB_PATH, ios::app);
	if (!dataBase.is_open())
		throw "Unable to open file.";
	dataBase << chrono::system_clock::to_time_t(CSV_Save::numFrame) << "," << avgR << "," << avgG << "," << avgB << "," << x << "," << y << "," << width << "," << height << std::endl;
	dataBase.close();
	Logging::getFile()->info("Averages written to averages.csv");
}