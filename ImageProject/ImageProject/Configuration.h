#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include "json.hpp"
#include "Logging.h"


//the values in the configuration file
enum  eConfig {windows_netPath, linux_netPath, queueSize, windows_saveToDBPath, linux_saveToDBPath, CSVPath, windows_objectsListPath, linux_objectsListPath, windows_videoPath, linux_videoPath};

class Configuration {
private:
	nlohmann::json config;
	inline static Configuration* configuration=nullptr;
public:
	 Configuration();
	 ~Configuration(){ }
     static Configuration& 	getConfiguration();
	 nlohmann::json getConfigValue(eConfig key)const;
	 Configuration(Configuration& ) = delete;
	 void operator=(const Configuration&) = delete;
};
