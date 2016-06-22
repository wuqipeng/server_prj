#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/helpers/stringhelper.h"
#include "log4cplus/loggingmacros.h"

#include "log.h"


using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace server::log;

Logger log_helper::logger;    //静态变量的初始化

void log_helper::initialize(const string& confFile, const string& logName)
{
    PropertyConfigurator::doConfigure(confFile);
    logger =  Logger::getInstance(logName);
}


 Logger& log_helper::get_logger()
 {
    return logger;
 }

 
void server::log::logger(const LOGLEVEL log_level, const string msg)
{	 
    Logger& logger = log_helper::get_logger();

    switch(log_level)
    {
        case LOG_TRACE:
            LOG4CPLUS_TRACE_STR(logger,msg);
            break;
        case LOG_DEBUG:
            LOG4CPLUS_DEBUG_STR(logger,msg);
            break;
        case LOG_INFO:
            LOG4CPLUS_INFO_STR(logger,msg);
            break;
        case LOG_WARN:
            LOG4CPLUS_WARN_STR(logger,msg);
            break;
        case LOG_ERROR:
            LOG4CPLUS_ERROR_STR(logger,msg);
            break;
        case LOG_FATAL:
            LOG4CPLUS_FATAL_STR(logger,msg);
            break;
    }
}

