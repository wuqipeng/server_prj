#include "Thread.h"
#include "lib/log.h"
#include "test_thread.h"

using namespace server::thread;
using namespace server::log;

void TestTask::run()
{
	logger(LOG_INFO, "this is a test run!");
}

int main(int , char** )
{
	log_helper::initialize("../../../bin/logConfig.properties", "server_logger");
    {
        MulThreadHelper test_helper;
        test_helper.Init(1,100);

        int i= 1;
        while(i--)
        {
            test_helper.PushTask(new TestTask(ENUM_TASK_TYPE_1));
        }
    }
}

