#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <spdlog/fmt/bundled/core.h>
#include "SaveData.h"
#include "Configuration.h"
using namespace std;
#define CSV_PATH  Configuration::getConfiguration().getConfigValue(eConfig::CSVPath)

class CSV_Save :public SaveData<ifstream>
{
private:
	static bool isFirst;
public:
	CSV_Save();
	~CSV_Save() {}
	void virtual save(double avgR, double avgG, double avgB, float x, float y, float width, float height);
	void operator =(CSV_Save& other) = delete;
};