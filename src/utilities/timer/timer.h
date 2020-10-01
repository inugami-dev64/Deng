#ifndef TIMER_H
#define TIMER_H

namespace dengUtils {
    
    class Timer {
    private:
        long getTime();
        long time_point;
        
    public:
        void setNewTimePoint();
        bool isTimePassed(const int &ms);
        void delay();
        Timer();
    };
}

#endif