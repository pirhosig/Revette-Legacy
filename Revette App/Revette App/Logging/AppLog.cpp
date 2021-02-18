#include "AppLog.h"
#include <iostream>


void AppLog::loadLog(const char* logFilename)
{
	logFile.open(logFilename);
	if (!logFile.is_open())
	{
		std::cout << "Failed to open log file" << std::endl;
	}
}



void AppLog::writeLog(const std::string message, const LOGMODE logType)
{
	const char* rawMessage = message.c_str();
	const size_t messageSize = message.size();

	// Add a newline
	logFile.write("\n", 1);

	switch (logType)
	{
	case LOGMODE::ERROR:
	{
		logFile.write("[ERROR] ", 9);
		logFile.write(rawMessage, messageSize);
		logFile.flush();
		break;
	}
	case LOGMODE::FATAL:
	{
		logFile.write("[FATAL] ", 9);
		logFile.write(rawMessage, messageSize);
		logFile.flush();
		break;
	}
	case LOGMODE::INFO:
	{
		logFile.write("[INFO] ", 8);
		logFile.write(rawMessage, messageSize);
		break;
	}
	case LOGMODE::FLUSHED_INFO:
	{
		logFile.write("[INFO] ", 8);
		logFile.write(rawMessage, messageSize);
		logFile.flush();
		break;
	}
	case LOGMODE::UNPREFIXED:
	{
		logFile.write(rawMessage, messageSize);
		break;
	}
	default:
	{
		logFile.write("[????] ", 8);
		logFile.write(rawMessage, messageSize);
		break;
	}
	}
}