#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include<list>


class MovingAverage {
    public:
    MovingAverage(int);
    ~MovingAverage();
    void addValue(double);
    double getLMA();
    private:
    std::list<double> * valList;
    int listSize;
    int maxSize;
};



#endif