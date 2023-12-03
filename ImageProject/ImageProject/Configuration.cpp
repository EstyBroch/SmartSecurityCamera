#include "Configuration.h"


#ifdef _WIN32
#define CONFIGURATION_PATH_FILE R"(C:\MobileyeBootcamp_MEEC\ImageProject\ImageProject\files\configure.json)"
#elif __linux__
#define CONFIGURATION_PATH_FILE "//mnt//c/MobileyeBootcamp_MEEC//ImageProject//ImageProject//files//configure.json"
#endif
using namespace std;
using namespace nlohmann;

//place the json file into the json private variable
Configuration::Configuration()
{
	Logging::getFile()->debug("configuration constructor");
	std::ifstream configFile(CONFIGURATION_PATH_FILE);
	if (configFile.fail())
		Logging::getFile()->error("Unable to open config file");
	else 
		configFile >> config;
	configFile.close();
}

Configuration& Configuration::getConfiguration() {
	Logging::getFile()->debug("get configuration instance");
	if (configuration == nullptr)
		configuration = new Configuration();
	return *configuration;
}

//get the json value by the key
json Configuration::getConfigValue(eConfig key)const {
	//Logging::getFile()->info("get Config Value {}",key);
	string val;
	switch (key)
	{
    case windows_saveToDBPath:
        val = "windows_saveToDBPath";
        break;
    case windows_netPath:
        val = "windows_netPath";
        break;
    case queueSize:
        val = "queueSize";
        break;
    case CSVPath:
        val = "CSVPath";
        break;
    case windows_objectsListPath:
        val = "windows_objectsListPath";
        break;
    case windows_videoPath:
        val = "windows_videoPath";
        break;
        case linux_netPath:
        val = "linux_netPath";
        break;
        case linux_saveToDBPath:
        val = "linux_saveToDBPath";
        break;
        case linux_objectsListPath:
        val = "linux_objectsListPath";
        break;
        case linux_videoPath:
        val = "linux_videoPath";
        cout<<"linux path"<<endl;
        break;
    default:
        Logging::getFile()->error("wrong key of config");
        return NULL; //check what is the deafault value
        break;
    }
	return config[val];
}