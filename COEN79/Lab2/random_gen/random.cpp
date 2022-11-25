#include <cassert>
#include <iostream>
#include "random.h"

using namespace std;
using namespace coen79_lab2;

namespace coen79_lab2 {
    // CONSTRUCTOR for the rand_gen class:
    rand_gen::rand_gen(int in_seed, int in_multiplier, int in_increment, int in_modulus) {
        //     Postcondition: The object has been initialized, and is ready to generate pseudorandom numbers.
        assert(in_modulus != 0);
        seed = in_seed;
        multiplier = in_multiplier;
        increment = in_increment;
        modulus = in_modulus;
    }
    // PUBLIC MODIFICATION member functions for the rand_gen class:
    void rand_gen::set_seed(int newSeed) {
        //    Postcondition: the number new_seed is the new seed
        seed = newSeed;
    }
    
    int rand_gen::next() {
        //    Postcondition: The return value is the next random integer, and the return value is set as the new seed
        int num = (multiplier * seed + increment) % modulus;
        seed = num;
        return num;
    }

    // PUBLIC CONSTANT member functions for the rand_gen class:
    void rand_gen::print_info() {
        //    Postcondition: Prints the values of the private member variables
        cout    << "Seed: " << seed << endl
                << "Multiplier: " << multiplier << endl
                << "Increment: " << increment << endl
                << "Modulus: " << modulus << endl;
    }
}