#include "../core/dengcore.h"

namespace deng {
    long Timer::getTime() {
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

    //Function that returns true if certain amount of time has passed 
    bool Timer::isTimePassed(const int &ms) {
        if(this->getTime() - this->time_point >= ms) return true;
        else return false; 
    }
}