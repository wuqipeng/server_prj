#include "Thread.h"

using namespace server::thread;

class TestTask:public Task
{
	public:
		TestTask(TaskType type_):Task(type_){}
		virtual void run();

};
