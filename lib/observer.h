#ifndef  OBSERVER_H_
#define OBSERVER_H_

#include <boost/thread/mutex.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "noncopyable.h"

namespace server { namespace lib {
   
    class Observer;
    class Observerable : public noncopyable
    {
        public:
            void registerObserver(boost::weak_ptr<Observer> observer_);
            void unRegisterObserver(boost::shared_ptr<Observer> observer_);
            void notifyObserver(const int32_t i);
            uint32_t size();
            
            std::vector<boost::weak_ptr<Observer> > observers_;
        private:
            boost::mutex mutex_;
            //std::vector<boost::weak_ptr<Observer> > observers_;
    };

    class Observer : public noncopyable, public boost::enable_shared_from_this<Observer>
    {
        public:
            Observer(boost::weak_ptr<Observerable> _Observerable_):Observerable_(_Observerable_){}
            void registerObser();
            virtual void update(const int32_t i);
        private:
            boost::weak_ptr<Observerable> Observerable_;
            int32_t value;
    };

}}
#endif
