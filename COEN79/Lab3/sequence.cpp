/*
*   INVARIANTS OF CLASS:
*   - "used" refers to the amount of items stored in a sequence
*   - when the sequence is empty, it does not matter what is in "data"; when non-empty,
*     items of interest are store from data[0] through data[used - 1]
*   - current item is accessed by data[current_index], if there is no current item,
*     current_index = used;
*/

#include "sequence.h"
#include <math.h>
#include <cassert>
#include <algorithm>

namespace coen79_lab3 {
    const sequence::size_type sequence::CAPACITY;

    // CONSTRUCTOR for the sequence class:
    sequence::sequence( ) {
        // Postcondition: The sequence has been initialized as an empty sequence.
        used = 0;
        current_index = used;
        value_type data[CAPACITY];
    }


    // MODIFICATION MEMBER FUNCTIONS for the sequence class:
    void sequence::start() { 
        // Poscondition: The iterator is reset to position 0
        current_index = 0; 
    }

    void sequence::end() {
        // Poscondition: The iterator points to the last item in the sequence
        if (used == 0) current_index = used; 
        else current_index = used - 1;
    }

    void sequence::last() { 
        // Poscondition: The iterator is reset to CAPACITY - 1, regardless of the numebr of items in the sequence
        current_index = CAPACITY - 1;
    }

    void sequence::advance() { 
        /* Precondtion: isitem() returns true
        Postcondition: The iterator's value is increased by 1, unless it is already at the end of the sequence. */
        assert(current_index <= used - 1);    
        ++current_index;
    }

    void sequence::retreat() {
        // Postcondition: The iterator's value is reduced by 1, unless it is already at the beginning of the sequence.
        assert(current_index > 0);
        --current_index;
    }
    
    void sequence::insert(const value_type& entry) { 
        /* Precondition: size() < CAPACITY
        Postcondition: A new copy of entry has been inserted in the sequence
        before the current item. If there was no current item, the new entry
        has been inserted at the front. In either case, the new item is now the
        current item of the sequence. */
        assert(size() < CAPACITY);
        size_type i;
        // check if empty
        if (used == 0) {
            data[0] = entry;
            start();
            ++used;
            return;
        }
        // shift items to make space for entry
        for (i = used; i > current_index; --i) {
            data[i] = data[i - 1];
        }
        data[i] = entry;
        ++used;
    }

    void sequence::attach(const value_type& entry) {
        /* Precondition: size() < CAPACITY
        Postcondition: A new copy of entry has been inserted in the sequence
        after the current item. If there was no current item, the new entry
        has been attached to the end. In either case, the new item is now the
        current item of the sequence. */
        assert(size() < CAPACITY);
        size_type i;
        // check if empty
        if (used == 0) {
            data[0] = entry;
            start();
            ++used;
            return;
        }
        // shift items to make space for entry
        for (i = used; i > current_index + 1; --i) {
            data[i] = data[i - 1];
        }
        data[i] = entry;
        advance();
        ++used;
    }

    void sequence::remove_current() {
        /* Precondtion: isitem() returns true
        Postcondition: The current item has been removed from the sequence,
        and the item after this (if there is one) is now the current item. */
        assert(is_item());
        // shift items to remove current item
        for (size_type i = current_index; i < used - 1; ++i) {
            data[i] = data[i + 1];
        }
        --used;
        // check if empty
        current_index = used == 0 ? used : current_index - 1; 
    }

    void sequence::insert_front(const value_type& entry) {
        /* Precondition: size() < CAPACITY
        Postcondition: A new copy of entry has been inserted in the sequence
        at the front. The new item is now the current item of the sequence. */
        assert(size() < CAPACITY);
        // check if empty
        if (used == 0) {
            data[0] = entry;
            start();
            ++used;
            return;
        }
        // shift items
        for (size_type i = used; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = entry;
        start();
        ++used;
    }

    void sequence::attach_back(const value_type& entry) {
        /* Precondition: size() < CAPACITY
        Postcondition: A new copy of entry has been inserted in the sequence
        at the back. The new item is now the current item of the sequence. */
        assert(size() < CAPACITY);
        data[used] = entry;
        ++used;
        end();
    }

    void sequence::remove_front() {
        /* Precondtion: isitem() returns true
        Postcondition: The item at the front of the sequence has been removed from the sequence,
        and the current item points at the front of the sequence. */
        assert(is_item());
        for (size_type i = 0; i < used - 1; ++i) {
            data[i] = data[i + 1];
        }
        start();
        --used;
    }

    void sequence::operator +=(const sequence& rhs) {
        // Postcondition: Adds the items of rhs to the lhs. Also returns a copy of lhs.
        assert(size() + rhs.size() <= CAPACITY);
        std::copy(rhs.data, rhs.data + rhs.used, data + used);
        used += rhs.used;
    }

        
    // CONSTANT MEMBER FUNCTIONS for the sequence class:
    sequence::size_type sequence::size() const {
        // Postcondition: The return value is the number of items in the sequence.
        return used;
    }

    bool sequence::is_item( ) const {
        /* Postcondition: A true return value indicates that there is a valid
        "current" item that may be retrieved by activating the current
        member function (listed below). A false return value indicates that
        there is no valid current item. */
        return (current_index != used) ? true : false;
    }

    sequence::value_type sequence::current() const {
        /* Precondtion: isitem() returns true
        Poscondition: Returns the item in the sequence at the current position of the iterator. */
        assert(is_item());
        return data[current_index];
    }

    sequence::value_type sequence::operator[](int index) const {
        /* Precondition: index < used
        Postcondition: The item returned is the item stored at "index" */
        assert(index < used);
        return data[index];
    }

    double sequence::mean() const {
        // Postcondition: The value returned is the mean of the values stored in the sequence.
        value_type sum = 0;
        // sum data
        for (size_type i = 0; i < used; ++i) {
            sum += data[i];
        }
        return sum / used;
    }

    double sequence::standardDeviation() const {
        // Postcondition: The value returned is the stadard deviation of the values stored in the sequence.
        value_type avg = mean(), sum = 0;
        // sum square difference
        for (size_type i = 0; i < used; ++i) {
            sum += pow(data[i] - avg, 2);
        }
        return sqrt(sum / used);
    }


    // NON-MEMBER functions for the statistician class:
    sequence operator +(const sequence& lhs, const sequence& rhs) {
        /* Postcondition: The sequence that is returned contains all the
        numbers of the sequences of lsh and rhs. */
        assert(lhs.size() + rhs.size() <= sequence::CAPACITY);
        sequence seq;
        seq += lhs;
        seq += rhs;
        return seq;
    }

    sequence::value_type summation(const sequence &s) {
        // Postcondition: The value returned is the summation of the values stored in the sequence s.
        return s.mean() * s.used;
    }
}