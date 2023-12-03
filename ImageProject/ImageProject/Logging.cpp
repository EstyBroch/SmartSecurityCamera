#include "Logging.h"

shared_ptr<spdlog::logger> Logging::logger = nullptr;

Logging::Logging() {
	//logger = spdlog::basic_logger_mt("logger", "log.txt");
	//spdlog::set_default_logger(logger);
	logger = spdlog::stdout_color_mt("logger");
	spdlog::set_level(spdlog::level::debug);
	logger->info("Logging constructor");
}

shared_ptr<spdlog::logger> Logging::getFile()
{
	if (logger == nullptr)
		Logging();
	logger->info("Logging get file");
	return logger;
}