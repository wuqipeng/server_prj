#include <boost/thread/mutex.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include "noncopyable.h"
#include "log.h"
#include "observer.h"
#include "string_helper.h"

using namespace std;
using namespace boost;
using namespace server::lib;
using namespace server::log;


static void threadMain(shared_ptr<Observerable> Observerable_, shared_ptr<boost::condition> com_cond, shared_ptr<boost::mutex> mutex_)
{
    shared_ptr<Observer> MyObserver(new Observer(Observerable_));
    MyObserver->registerObser();
    
    mutex::scoped_lock sl(*mutex_);
    com_cond->wait(sl);
    
    logger(LOG_INFO, "thread exit!");
}

int main(int sz, char** args)
{
	log_helper::initialize("../bin/logConfig.properties", "server_logger");
    
    shared_ptr<Observerable> MyObserverable(new Observerable);
    shared_ptr<boost::condition> com_cond(new boost::condition);
    shared_ptr<boost::mutex> mutex_(new boost::mutex);
    int32_t i = 10;
    
    vector<shared_ptr<boost::thread> > threads_;
    while(i)
    {
        threads_.push_back(shared_ptr<boost::thread>(new boost::thread(bind(&threadMain,MyObserverable, com_cond, mutex_))));
        i--;
    }

    int32_t j = 10;
    while(j)
    {
        logger(LOG_INFO,"observer sum is " + uToStr(MyObserverable->size()));
        MyObserverable->notifyObserver(j);
        com_cond->notify_one();        
        j--;
    }

    for(vector<shared_ptr<boost::thread> >::iterator it = threads_.begin(); it != threads_.end(); it++)
    {
        (*it)->join();
    }
}

