#pragma once
#include <iostream>
#include <chrono>
#include "Configuration.h"
#ifdef _WIN32
#define SAVE_TO_DB_PATH Configuration::getConfiguration().getConfigValue(eConfig::windows_saveToDBPath)
#else
#define SAVE_TO_DB_PATH Configuration::getConfiguration().getConfigValue(eConfig::linux_saveToDBPath)
#endif

template<typename T>
class SaveData
{
protected:
	inline static T dataBase;
	inline auto static numFrame = std::chrono::system_clock::now();
    inline bool static isFirstInstance = true;
public:
	SaveData() = default;
	~SaveData() = default;
	virtual void save(double avgR, double avgG, double avgB, float x, float y, float width, float height) = 0;

};