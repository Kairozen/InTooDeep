#ifndef CLOCK_MANAGER_HPP
#define CLOCK_MANAGER_HPP

#include <iostream>
#include <deque>

#define NB_TIMES 20

using namespace std;

class ClockManager
{
public:
    deque<float> times;

    ClockManager();
    ~ClockManager();

    void add(float deltaTime);

    float average();
};

#endif