#define __TIMER_CPP
#include <utils/timer.h>

namespace dengUtils {
    deng_ui32_t Timer::getTime() {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();
        return dtn.count() / 1000000;
    }

    //These functions set new time point from epoch
    void Timer::setNewTimePoint() {
        this->time_point = this->getTime();
    }

    Timer::Timer() {
        this->setNewTimePoint();
    }

    // Function that returns true if certain amount of time has passed 
    deng_bool_t Timer::isTimePassed(const deng_ui32_t &ms) {
        if(getTime() - time_point >= ms) return true;
        else return false; 
    }
}
