#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string.h>
#include <chrono>
#include <iomanip> 
#include <ctime>
#include <string>
#include "Configuration.h"
#include "SaveData.h"
#include "../../ImageProject/sqlite_files/sqlite3.h"

using namespace std;
using namespace chrono;

class Sqlite :public SaveData<sqlite3*>
{
public:
	Sqlite();
	~Sqlite() {}
	void virtual save(double avgR, double avgG, double avgB, float x, float y, float width, float height);
	int createDB();
	int createTable();
};