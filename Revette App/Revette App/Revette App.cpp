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
        GlobalAppLog.fatalClose();
        return -1;
    }
    catch (...)
    {
        GlobalAppLog.writeLog("NO IDEA WHAT THE FUCK WENT WRONG, BUT SHIT'S DONE FOR", LOGMODE::FATAL);
        GlobalAppLog.fatalClose();
        return -2;
    }
    
    // Log program finish
    GlobalAppLog.writeLog("Program exit", LOGMODE::FLUSHED_INFO);

    return returnCode;
}