#define __TIMER_CPP
#include <utils/timer.h>

namespace dengUtils {
    deng_ui64_t Timer::getTime() {
        return (deng_ui64_t)std::chrono::duration_cast<std::chrono::milliseconds> 
            (std::chrono::system_clock::now().time_since_epoch()).count();
    }

    //These functions set new time point from epoch
    void Timer::setNewTimePoint() {
        m_time_point = getTime();
    }

    Timer::Timer() {
        this->setNewTimePoint();
    }

    // Function that returns true if certain amount of time has passed 
    deng_bool_t Timer::isTimePassed(const deng_ui64_t &ms) {
        if(getTime() - m_time_point >= ms) return true;
        else return false; 
    }
}
