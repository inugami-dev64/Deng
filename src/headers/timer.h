#include "parsing.h"

namespace deng {
    class Timer {
    private:
        long getTime();
        long timePoint;
        
    public:
        void setNewTimePoint();
        bool isTimePassed(const int &ms);
        void delay();
        Timer();
    };
}