#include "App.h"
#include <exception>
#include "Logging/AppLog.h"

/* Revette App.cpp : This file contains the 'main' function. Program execution begins and ends here.
*/


AppLog GlobalAppLog;


int main()
{
    // Load the log file for the whole application
    GlobalAppLog.loadLog("log.txt");

    // Log application starting
    GlobalAppLog.writeLog("Application starting...", LOGMODE::INFO);

    App app;
    int returnCode;
    try
    {
        returnCode = app.run();
    }
    catch (std::exception const& e)
    {
        GlobalAppLog.writeLog(e.what(), LOGMODE::FATAL);
        GlobalAppLog.writeLog("SHIT'S FUCKED. GOODBYE.", LOGMODE::FATAL);
        return -1;
    }
    
    // Log program finish
    GlobalAppLog.writeLog("Program exit", LOGMODE::FLUSHED_INFO);

    return returnCode;
}