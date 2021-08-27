#include "MovingAverage.hpp"
#include <math.h>


MovingAverage::MovingAverage(int size){
    this->maxSize = size;
    this->valList = new std::list<double>();
}

MovingAverage::~MovingAverage(){
    delete(this->valList);
}


void MovingAverage::addValue(double val){
    this->valList->push_front(val);
    if (this->valList->size() > this->maxSize){
        this->valList->pop_back();
    }
}

double MovingAverage::getLMA(){
    if (this->valList->size() == 0){
        return 0;
    }

    double div = 10;
    long double deLogSum = 0;
    for(auto it = this->valList->begin(); it != this->valList->end(); ++it){
        deLogSum += (long double) pow(10, *it/div);
    }
    

    return div * log10(deLogSum/(long double) this->valList->size());
    
}