#include <boost/thread/mutex.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "noncopyable.h"
#include "log.h"
#include "observer.h"
#include "string_helper.h"

using namespace std;
using namespace boost;
using namespace server::lib;
using namespace server::log;

void Observerable::registerObserver(boost::weak_ptr<Observer> observer_)
{
    mutex::scoped_lock sl(mutex_);
    observers_.push_back(observer_);
    logger(LOG_INFO, "registerObserver ,size is " + uToStr(observers_.size()));
}

void Observerable::unRegisterObserver(shared_ptr<Observer> observer_)
{
    mutex::scoped_lock sl(mutex_);
    for(vector<boost::weak_ptr<Observer> >::iterator it = observers_.begin(); it != observers_.end(); )
    {
        shared_ptr<Observer> tmp_ptr = it->lock();
        if(tmp_ptr)
        {
            if(tmp_ptr == observer_)
            {
                observers_.erase(it);
                break;
            }
            it++;
        }else
        {
            observers_.erase(it);
        }

    }
}

void Observerable::notifyObserver(const int32_t i)
{
    mutex::scoped_lock sl(mutex_);
    for(vector<boost::weak_ptr<Observer> >::iterator it = observers_.begin(); it != observers_.end(); )
    {
        shared_ptr<Observer> tmp_ptr = it->lock();
        if(tmp_ptr)
        {
            logger(LOG_INFO,"Observer valid!");
            tmp_ptr->update(i);
            it++;
        }else
        {
            logger(LOG_INFO,"Observer invalid!");
            observers_.erase(it);
        }
    }
}

uint32_t Observerable::size()
{
    return observers_.size();
}

void Observer::registerObser()
{
    shared_ptr<Observerable> tmp_ptr = Observerable_.lock();
    if(tmp_ptr)
    {
        tmp_ptr->registerObserver(shared_from_this());
    }
}

void Observer::update(const int32_t i)
{
    value = i;
    logger(LOG_INFO, "Observerable notify i : " + uToStr(value));
}


