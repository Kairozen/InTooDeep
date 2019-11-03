#ifndef CLOCK_MANAGER_CPP
#define CLOCK_MANAGER_CPP

#include "ClockManager.hpp"

ClockManager::ClockManager() 
{
    times = deque<float>();
}

ClockManager::~ClockManager() {}

void ClockManager::add(float deltaTime)
{
    if(times.size() >= 20)
        times.pop_front();
    times.push_back(deltaTime);
}

float ClockManager::average()
{
    float sum = 0;
    for(deque<float>::iterator it = times.begin(); it != times.end(); ++it)
        sum += *it;
    return sum/times.size();
}


#endif