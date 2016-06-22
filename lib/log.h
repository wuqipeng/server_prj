#ifndef LOG_H_ 
#define LOG_H_

#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/helpers/stringhelper.h"
#include "log4cplus/loggingmacros.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

namespace server{ namespace log {

	typedef enum {
	 	LOG_TRACE,
		LOG_DEBUG,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR,
		LOG_FATAL
	 }LOGLEVEL;


	class log_helper {
		public:
			static void initialize(const string& confFile, const string& logName);
			static Logger& get_logger();
	
		private:
			static Logger logger;
	};

    void logger(const LOGLEVEL log_level, const string msg);


}}


#endif

