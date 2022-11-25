/*
*   INVARIANTS OF CLASS:
*   - "used" refers to the amount of items stored in a bag (keys and data)
*   - when the bag is empty, it does not matter what is in "data" or "keys"; when non-empty,
*     items of interest are stored from data[0] through data[used - 1]
*   - "keys" and "data" array are in parallel; i.e. data[0] corresponds to keys[0]...
*/

#include "keyed_bag.h"
#include <algorithm>
#include <iostream>

using namespace std;

namespace coen79_lab4 {
    const keyed_bag::size_type keyed_bag::CAPACITY;
    
    // CONSTRUCTOR for the keyed_bag class:
    keyed_bag::keyed_bag() {
        // Postcondition: The keyed_bag has been initialized as an empty keyed_bag.
        key_type keys[CAPACITY];
        value_type data[CAPACITY];
        used = 0;
    }


    // MODIFICATION MEMBER FUNCTIONS for the keyed_bag class:
    void keyed_bag::erase() {
        // Postcondition: This keyed_bag is emptied of all items and keys.
        used = 0;
    }

    bool keyed_bag::erase(const key_type& key) {
        /* Postcondition: If key was in the keyed_bag, then the key and its value have been removed;
        otherwise the keyed_bag is unchanged. A true return value indicates that an
        item was removed; false indicates that nothing was removed. */
        if (!has_key(key)) return false;
        if (keys[--used] == key) {
            return true;
        }
        for (size_type i = 0; i < used + 1; ++i) {
            // find key value and swap with last item then delete
            if (keys[i] == key) {
                data[i] = data[used];
                keys[i] = keys[used];
            }
        }
        return true;
    }

    void keyed_bag::insert(const value_type& entry, const key_type& key) {
        /* Precondition:  size() < CAPACITY, and the keyed_bag does not yet contain
        the given key.
        Postcondition: A new copy of entry has been added to the keyed_bag with
        the corresponding key. */
        assert((size() < CAPACITY) && (!has_key(key)));
        keys[used] = key;
        data[used++] = entry;
    }

    void keyed_bag::operator +=(const keyed_bag& addend) {
        /* Precondition:  size( ) + addend.size( ) <= CAPACITY.
        The intersection of the keyed_bags' key tables is empty (i.e. the keyed_bags
        share no keys.)
        Postcondition: Each item and its key in addend has been added to this keyed_bag. */
        assert((size() + addend.size()) <= CAPACITY);
        assert(!hasDuplicateKey(addend));
        for (size_type i = 0; i < addend.used ; ++i) {
            data[used + i] = addend.data[i];
            keys[used + i] = addend.keys[i];
        }
        used += addend.used;
    }


    // CONSTANT MEMBER FUNCTIONS for the keyed_bag class:
    bool keyed_bag::has_key(const key_type& key) const {
        /* Postcondition: The return value is whether or not the given key is in
        the keyed_bag's list of keys. */
        for (size_type i = 0; i < used; ++i) {
            // return true if key is found
            if (keys[i] == key) return true;
        }
        return false;
    }

    keyed_bag::value_type keyed_bag::get(const key_type& key) const {
        /* Precondition: has_key returns true for the given key.
        Postcondtion: the data value corresponding to the given key is returned. */
        assert(has_key(key));
        size_type i;
        for (i = 0; i < used; ++i) {
            // if key is found break and return data with passed key value
            if (keys[i] == key) break;
        }
        return data[i];
    }

    keyed_bag::size_type keyed_bag::size() const {
        // Postcondition: The return value is the total number of items in the keyed_bag.
        return used;
    }

    keyed_bag::size_type keyed_bag::count(const value_type& target) const {
        // Postcondition: The return value is number of times target is in the keyed_bag.
        size_type cnt = 0;
        for (size_type i = 0; i < used; ++i) {
            // tally data equal to passed target
            if (data[i] == target) ++cnt;
        }
        return cnt;
    }

    bool keyed_bag::hasDuplicateKey(const keyed_bag& otherBag) const {
        /* Postcondition: The return value is true if there is at least one key used by both this 
        bag and the "otherBag". In other words, this function checks if there is any shared key by 
        the two bags. */
        for (size_type i = 0; i < otherBag.used; ++i) {
            // return true if keyed_bag has any matching keys with otherBag
            if (has_key(otherBag.keys[i])) return true;
        }
        return false;
    }

    // NONMEMBER FUNCTIONS for the keyed_bag class:
    keyed_bag operator +(const keyed_bag& b1, const keyed_bag& b2) {
        /* Precondition:  b1.size( ) + b2.size( ) <= keyed_bag::CAPACITY.
                    The intersection of the keyed_bags' key tables is empty.
                    (i.e. The two keyed_bag's have no keys in common.)
        Postcondition: The keyed_bag returned is the union of b1 and b2. */
        assert(b1.size( ) + b2.size( ) <= keyed_bag::CAPACITY);
        keyed_bag kb;
        kb += b1;
        kb += b2;
        return kb;
    }
}