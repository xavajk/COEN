// FILE: statistician.cpp
// CLASS IMPLEMENTED: statistician (see stats.h for documentation)

#include <cassert>   // Provides assert
#include <iostream>  // Provides istream class
#include "statistician.h"

using namespace std;
using namespace coen79_lab2;

namespace coen79_lab2 {
    // CONSTRUCTOR for the statistician class:
    statistician::statistician( ) {
        length = 0;
        sum = 0;
    }

    // PUBLIC MODIFICATION member functions for the statistician class:
    void statistician::next(double r) {
        if (length == 0)   { smallest = r; largest = r; } 
        if (r < smallest)  { smallest = r; }
        if (r > largest)   { largest = r; }
        sum += r;
        last = r;
        length++;
    }
    void    statistician::reset()               { length = 0; sum = 0; }

    // PUBLIC CONSTANT member functions for the statistician class:
    int     statistician::stat_length() const   { return length; }
    double  statistician::stat_sum()    const   { return sum; }
    double  statistician::mean()        const   { return sum / length; }
    double  statistician::minimum()     const   { return smallest; }
    double  statistician::maximum()     const   { return largest; }

    // NON-MEMBER functions for the statistician class:
    statistician operator +(const statistician& s1, const statistician& s2) {
        //     Postcondition: The statistician that is returned contains all the
        //     numbers of the sequences of s1 and s2.
        statistician stat;
        stat.length = s1.length + s2.length;
        stat.sum = s1.sum + s2.sum;
        stat.last = s1.last + s2.last;
        stat.smallest = s1.smallest < s2.smallest ? s1.smallest : s2.smallest;
        stat.largest = s1.largest > s2.largest ? s1.largest : s2.largest;
        return stat;
    }

    statistician operator *(double scale, const statistician& s) {
        //     Postcondition: The statistician that is returned contains the same
        //     numbers that s does, but each number has been multiplied by the
        //     scale number.
        statistician stat;
        stat.length     = s.stat_length();
        stat.sum        = scale * s.stat_sum();
        stat.last       = scale * s.last;
        stat.smallest   = scale * s.maximum();
        stat.largest    = scale * s.minimum();
        return stat;
    }

    bool operator ==(const statistician& s1, const statistician& s2) {
        //     Postcondition: The return value is true if s1 and s2 have the zero
        //     length. Also, if the length is greater than zero, then s1 and s2 must
        //     have the same length, the same  mean, the same minimum, 
        //     the same maximum, and the same sum.
        if (s1.length == 0 and s2.length == 0) return true;
        else if (   s1.stat_length()    == s2.stat_length()     and
                    s1.mean()           == s2.mean()            and
                    s1.minimum()        == s2.minimum()         and
                    s1.maximum()        == s2.maximum()         and
                    s1.stat_sum()       == s2.stat_sum()
                ) return true;
        else return false;
    }
}