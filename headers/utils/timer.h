#ifndef __TIMER_H
#define __TIMER_H

#ifdef __TIMER_CPP
    #include <chrono>   
    #include <common/base_types.h>
#endif

namespace dengUtils {
    
    class Timer {
    private:
        deng_ui32_t getTime();
        deng_ui32_t time_point;
        
    public:
        void setNewTimePoint();
        deng_bool_t isTimePassed(const deng_ui32_t &ms);
        void delay();
        Timer();
    };
}

#endif
