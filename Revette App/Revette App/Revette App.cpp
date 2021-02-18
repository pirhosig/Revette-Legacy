/* Revette App.cpp : This file contains the 'main' function. Program execution begins and ends here.
*/

#include "App.h"
#include "Logging/AppLog.h"

AppLog GlobalAppLog;

int main()
{
    // Load the log file for the whole application
    GlobalAppLog.loadLog("log.txt");

    // Log application starting
    GlobalAppLog.writeLog("Application starting...", LOGMODE::INFO);

    App app;
    int result = app.run();
    
    // Log program finish
    GlobalAppLog.writeLog("Program exit", LOGMODE::FLUSHED_INFO);

    return result;
}