namespace deng {
    
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