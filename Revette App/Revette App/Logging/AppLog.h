#pragma once
#include <fstream>

enum class LOGMODE
{
	ERROR,
	FATAL,
	FLUSHED_INFO,
	INFO,
	UNPREFIXED
};



class AppLog
{
public:
	void loadLog(const char* logFilename);
	void writeLog(const std::string message, const LOGMODE logType);

private:
	std::ofstream logFile;
};