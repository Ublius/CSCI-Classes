#ifndef DIE_HPP
#define DIE_HPP

#include <cstdlib>

class Die 
{
    public:
    /**
    * @brief
    */
    int roll();
};

int Die::roll()
{
    // rolls a die between 0 and 7
    int x = rand() % (7-0+1) + 0;
    return x;
}

#endif