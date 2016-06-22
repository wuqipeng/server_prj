#include "Thread.h"
#include "log.h"
#include "string_helper.h"

using namespace std;
using namespace boost;
using namespace server::log;
using namespace server::thread;


// 少了boost，则编译出错???
BoostThread::BoostThread(ThreadManger * p_thread_manger):
	m_thread(new boost::thread(bind(&BoostThread::ThreadMain,this,p_thread_manger)))
{

}

void BoostThread::ThreadMain(ThreadManger * p_thread_manger)
{

	p_thread_manger->ThreadRun();
	p_thread_manger->ThreadExit(this);
	
}

void ThreadManger::IncreaseThread()
{
	thread_pool.insert(new BoostThread(this));
}

void ThreadManger::DecreaseThread()
{
	Task * T = new Task(ENUM_TASK_EXIT);
	PushTask(T);	
}

void ThreadManger::ThreadExit(BoostThread * p_thread)
{
	thread_pool.erase(p_thread);
	delete p_thread;

	if(thread_pool.empty())
	{
		thread_cond.notify_all(); //通知主线程，线程池的线程都释放了 
	}
}


void ThreadManger::SetThreadCount(const int32_t count)
{
	mutex::scoped_lock sl(thread_mutex);
	int32_t thread_sum = thread_pool.size();
	if( thread_sum < count )
	{
		for(int32_t i = thread_sum; i < count; i++)
		{
			IncreaseThread();
		}

	}else if( thread_sum > count )
	{
		for(int32_t i = thread_sum; i > count; i--)
		{
			DecreaseThread();
		}
	}
}

void ThreadManger::JoinAll()
{
	{
        string fisrt = "ThreadMange destruct: first, thread_pool size is " + uToStr(thread_pool.size());
		logger(LOG_INFO, fisrt);
        mutex::scoped_lock sl(thread_mutex);
		int32_t thread_sum = thread_pool.size();
		
		for(int32_t i = thread_sum; i > 0; i--)
		{

	        logger(LOG_INFO, "DecreaseThread!");
			DecreaseThread();
		}

		while(!thread_pool.empty())
		{
			thread_cond.wait(sl);
		}
        
	}

	mutex::scoped_lock sl(queue_mutex);
	while(!task_queue.empty())
	{
		Task* p_task = task_queue.front();
		task_queue.pop_front();
		delete p_task;
        string info = "ThreadMange destruct: thrid, thread_pool size is " + uToStr(task_queue.size());
        logger(LOG_INFO, info);
    }

}

void ThreadManger::PushTask(Task* task)
{
	mutex::scoped_lock sl(queue_mutex);
	int32_t num = task_queue.size();
	if(num >= queue_limit)
	{
		logger(LOG_WARN, "drop task, task queue more than " + uToStr(queue_limit));
		delete task;
		return;
	}

	task_queue.push_back(task);
	queue_cond.notify_one();	
}

void ThreadManger::ThreadRun()
{
	while(1)
	{
		mutex::scoped_lock sl(queue_mutex);
	
		if(task_queue.empty())
		{
			queue_cond.wait(sl);
		}

		Task* p_task = task_queue.front();
		task_queue.pop_front();
		if( p_task->GetTaskType() == ENUM_TASK_EXIT)
		{
		    logger(LOG_INFO, "Thread exit!");
			delete p_task;
			break;
		}

		p_task->run();
		delete p_task;
	}
}

void ThreadManger::SetQueueLimit(int32_t max_limit)
{
	queue_limit = max_limit;
}

void MulThreadHelper::Init(int32_t thread_num, int32_t queue_limit)
{
	_thread_manger.SetThreadCount(thread_num);
	_thread_manger.SetQueueLimit(queue_limit);
}

void MulThreadHelper::PushTask(Task* task)
{
	_thread_manger.PushTask(task);
}

