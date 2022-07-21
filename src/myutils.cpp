#include <stdlib.h>
#include <time.h>

#include "myutils.hpp"


/*产生-0.5~0.5之间的随机数*/
double myrand_double()
{
    static unsigned int seed = 0;
    ++seed;

    srand((unsigned)time(NULL)+seed*seed);

    return (rand()*1.0/(RAND_MAX)) - 0.5;
}

