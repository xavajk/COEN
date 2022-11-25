//FILE: sequence.cpp
//CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
//
//
//  COEN 79
//  --- Behnam Dezfouli, COEN, SCU ---
//
//
//INVARIANT for the sequence class
//  1. The number of items in the sequence is stored in the member variable "many_nodes"
//
//  2. For an empty sequence, we do not care what is stored in any of data and head_ptr
//      and tail_ptr point to NULL; for a non-empty sequence, the items stored in the
//      sequence can be accessed using head_ptr and tail_ptr and we don't care what is
//      stored in the rest of the data.
//
//  3. If there is a current item, then it lies in *cursor; if there is no
//     current item, then cursor* equals NULL.
//
//  4. If there is a previous item, then it lies in precursor*.  If there is no previous
//       item, then precursor equals NULL.


#include <iostream>
#include <algorithm> //provides copy function
#include <cassert> //for assert function
#include "sequence.h" //header file for class
#include "node.h" // provides node class

using namespace std; //For copy function

namespace coen79_lab6
{
    // PRIVATE INTIALIZATION FUNCTION
    void sequence::init()
    {
        // initializing pointers to NULL
        head_ptr = NULL;
        tail_ptr = NULL;
        cursor = NULL;
        precursor = NULL;
        many_nodes = 0;
    }


    // COPY CONSTRUCTOR
    sequence::sequence(const sequence& source)
    {
        init();
        *this = source;
    }

    // DESTRUCTOR
    sequence::~sequence()
    {
        list_clear(head_ptr);
        many_nodes = 0;
    }


    // MODIFICATION MEMBER FUNCTIONS
    void sequence::start()
    {
        cursor = head_ptr;
        precursor = head_ptr;
    }

    void sequence::end()
    {
        while (precursor->link() != tail_ptr) precursor = precursor->link();
        cursor = tail_ptr;
    }

    void sequence::advance()
    {   
        // advance if there is a current item
        if (is_item() == true)
        {
            // if cursor is at the end of sequence set cursor to NULL
            if (cursor == tail_ptr)
            {
                precursor = tail_ptr;
                cursor = NULL;
            }

            // else update links normally
            else 
            {
                precursor = cursor;
                cursor = cursor->link();
            }
        }
    }

    void sequence::insert(const value_type& entry) 
    {
        if (is_item() == false)
        {
            // if the sequence is empty
            if (head_ptr == NULL)
            {
                list_head_insert(head_ptr, entry);
                tail_ptr = head_ptr;
                cursor = head_ptr;
                precursor = NULL;
            }

            // if cursor is NULL but precursor is at end of sequence
            else if (precursor == tail_ptr)
            {
                list_head_insert(head_ptr, entry);
                cursor = head_ptr;
                precursor = NULL;
            }
        }

        // if cursor is valid
        else 
        {
            // if cursor is at head
            if (precursor == NULL)
            {
                list_head_insert(head_ptr, entry);
                cursor = head_ptr;
            }

            // else cursor is somewhere in the middle
            else
            {
                list_insert(precursor, entry);
                cursor = precursor->link();
            }
        }


        // increment node counter
        ++many_nodes;
    }

    void sequence::attach(const value_type& entry)
    {   
        // cursor does not point to a valid node
        if (is_item() == false)
        {
            // if the list is empty at to start of list (head) and update links
            if (head_ptr == NULL)
            {
                list_head_insert(head_ptr, entry);
                cursor = head_ptr;
                tail_ptr = head_ptr;
                precursor = NULL;
            }

            // else attach to end of list and update links
            else
            {
                list_insert(tail_ptr, entry);
                cursor = tail_ptr->link();
                precursor = tail_ptr;
                tail_ptr = tail_ptr->link();
            }
        }

        // else cursor is pointing to valid node
        else 
        {
            // insert after cursor and update links
            list_insert(cursor, entry);
            if (cursor == tail_ptr) tail_ptr = cursor->link();
            precursor = cursor;
            cursor = cursor->link();
        }

        // increment node counter
        ++many_nodes;
    }

    void sequence::operator =(const sequence& source)
    {
        // clear current list, then copy source into list
        if (head_ptr != NULL) list_clear(head_ptr);
        list_copy(source.head_ptr, head_ptr, tail_ptr);

        // set up temporary cursors
        node *src_cursor = source.head_ptr;
        node *new_cursor = head_ptr;

        // loop through source and new sequence simultaneously to set cursor and precursor
        while (src_cursor != NULL || new_cursor != NULL)
        {
            // set cursor and precursor if the source cursor link equals the cursor pointer
            if (src_cursor == source.precursor)
            {
                precursor = new_cursor;
                cursor = precursor->link();
            }

            // move temp cursors to next link
            src_cursor = src_cursor->link();
            new_cursor = new_cursor->link();
        }

        tail_ptr = new_cursor;
        many_nodes = source.many_nodes;
    }

	void sequence::remove_current()
    {
        // ensure that there is a valid current item
        assert(is_item() == true);

        // if cursor is at head node
        if (cursor == head_ptr) 
        {   
            precursor = head_ptr->link();
            list_head_remove(head_ptr);
            cursor = head_ptr;
        }
        
        // else cursor is in position [2, many_nodes]
        else 
        {
            // if cursor is at the end
            if (cursor == tail_ptr)
            {
                tail_ptr = precursor;
                cursor = NULL;
            }

            // move cursor to next node (node after node to remove)
            else
            {
                cursor = cursor->link();
            }

            // remove precursor->link()
            list_remove(precursor);
        }

        // decrement node counter
        --many_nodes;
    }


    // CONSTANT MEMBER FUNCTIONS
    sequence::value_type sequence::current() const
    {
        assert(is_item() == true);
        return cursor->data();
    }
}
