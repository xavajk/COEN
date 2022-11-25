// FILE: node.cpp
//
//
//  COEN 79
//  --- Behnam Dezfouli, COEN, SCU ---
//
//
// IMPLEMENTS: The functions of the node class and the
// linked list toolkit (see node1.h for documentation).
//
// INVARIANT for the node class:
//   The data of a node is stored in data_field
//   and the link to the next node is stored in link_field.

#include "node.h"
#include <cassert>    	// Provides assert
#include <cstdlib>    	// Provides NULL and size_t
#include <iostream>	  	// Provides cout

using namespace std;

namespace coen79_lab6
{
    size_t list_length(const node* head_ptr)
    // Library facilities used: cstdlib
    {
		const node *cursor;
		size_t answer;

		answer = 0;
		for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
			++answer;

		return answer;
    }

    void list_head_insert(node*& head_ptr, const node::value_type& entry)
    {	
    	head_ptr = new node(entry, head_ptr);
    }

    void list_insert(node* previous_ptr, const node::value_type& entry)
    {
    	node *insert_ptr;

    	insert_ptr = new node(entry, previous_ptr->link( ));
    	previous_ptr->set_link(insert_ptr);
    }

    node* list_search(node* head_ptr, const node::value_type& target)
    // Library facilities used: cstdlib
    {
    	node *cursor;

    	for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
    	    if (target == cursor->data( ))
        		return cursor;

    	return NULL;
    }

    const node* list_search(const node* head_ptr, const node::value_type& target)
    // Library facilities used: cstdlib
    {
    	const node *cursor;

    	for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
    	    if (target == cursor->data( ))
        		return cursor;

    	return NULL;
    }

    node* list_locate(node* head_ptr, size_t position)
    // Library facilities used: cassert, cstdlib
    {
    	node *cursor;
    	size_t i;

    	assert (0 < position);
    	cursor = head_ptr;
    	for (i = 1; (i < position) && (cursor != NULL); i++)
    	    cursor = cursor->link( );

    	return cursor;
    }

    const node* list_locate(const node* head_ptr, size_t position)
    // Library facilities used: cassert, cstdlib
    {
    	const node *cursor;
    	size_t i;

    	assert (0 < position);
    	cursor = head_ptr;
    	for (i = 1; (i < position) && (cursor != NULL); i++)
    	    cursor = cursor->link( );

    	return cursor;
    }

    void list_head_remove(node*& head_ptr)
    {
    	node *remove_ptr;
    	remove_ptr = head_ptr;
    	head_ptr = head_ptr->link( );
    	delete remove_ptr;
    }

    void list_remove(node* previous_ptr)
    {
    	node *remove_ptr;
    	remove_ptr = previous_ptr->link( );
    	previous_ptr->set_link( remove_ptr->link( ) );
    	delete remove_ptr;
    }

    void list_clear(node*& head_ptr)
    // Library facilities used: cstdlib
    {
    	while (head_ptr != NULL)
    	    list_head_remove(head_ptr);
    }

    void list_copy(const node* source_ptr, node*& head_ptr, node*& tail_ptr)
    // Library facilities used: cstdlib
    {
    	head_ptr = NULL;
    	tail_ptr = NULL;

    	// Handle the case of the empty list.
    	if (source_ptr == NULL) return;

    	// Make the head node for the newly created list, and put data in it.
    	list_head_insert(head_ptr, source_ptr->data());
    	tail_ptr = head_ptr;

    	// Copy the rest of the nodes one at a time, adding at the tail of new list.
    	source_ptr = source_ptr->link( );
    	while (source_ptr != NULL) 
		{
    	    list_insert(tail_ptr, source_ptr->data());
    	    tail_ptr = tail_ptr->link( );
    	    source_ptr = source_ptr->link( );
    	}
    }

	void list_piece(node* start_ptr, node* end_ptr, node*& head_ptr, node*& tail_ptr)
	{	
		// check that requirements are met to copy list piece
		if (start_ptr != NULL && start_ptr != end_ptr) 
		{
			// initialize head of copy
			head_ptr = new node(start_ptr->data(), start_ptr->link());

			// set up node pointers (cursors)
			node *new_cursor = head_ptr;
			node *src_cursor = start_ptr;

			// loop through source list and copy valid nodes
			while (src_cursor->link() != end_ptr && src_cursor != end_ptr)
			{
				// create new node, establish links
				node *n = new node(src_cursor->link()->data());
				new_cursor->set_link(n);
				new_cursor = new_cursor->link();
				src_cursor = src_cursor->link();
				tail_ptr = new_cursor;
			}
		}
	}

    size_t list_occurrences(node* head_ptr, const node::value_type& target)
	{
		// check if list is empty
		if (head_ptr == NULL) return 0;

		// initialize cursor and counter
		node *cursor = head_ptr;
		size_t count = 0;

		// loop through list and count matches
		while (cursor != NULL)
		{	
			// increment count if match, move cursor
			if (cursor->data() == target) ++count;
			cursor = cursor->link();
		}

		return count;
	}

    void list_insert_at(node*& head_ptr, const node::value_type& entry, size_t position)
	{
		assert(position > 0 && position <= list_length(head_ptr) + 1);
		node *cursor = head_ptr;

		// check if inserting at head
		if (position == 1) 
		{
			// establish links
			node *n = new node(entry, cursor);
			head_ptr = n;
		} 
		
		// else, insert node at position
		else 
		{
			// move cursor to node before desired position
			for (int i = 1; i < position - 1; ++i) cursor = cursor->link();
			node *n = new node(entry, cursor->link());
			cursor->set_link(n);			
		}
	}

    node::value_type list_remove_at(node*& head_ptr, size_t position)
	{
		assert(position > 0 && position <= list_length(head_ptr));
		node::value_type val;
		node *cursor = head_ptr;

		// if removing head, get head data then remove head
		if (position == 1) 
		{
			val = head_ptr->data();
			head_ptr = cursor->link();
			delete cursor;
			return val;
		} 
		
		// else get prev node, set ret, and remove node at position
		else 
		{
			// move cursor to node before desired position, get data, remove, update links
			for (int i = 1; i < position - 1; ++i) cursor = cursor->link();
			node *remove_ptr = cursor->link();
			cursor->set_link(remove_ptr->link());
			val = remove_ptr->data();
			delete remove_ptr;
			return val;
		}

	}

    node* list_copy_segment(node* head_ptr, size_t start, size_t finish)
	{
		assert((1 <= start) && (start <= finish) && (finish <= list_length(head_ptr)));

		// set up node pointers
		node *start_ptr = list_locate(head_ptr, start);
		node *end_ptr = list_locate(head_ptr, finish);
		node *cursor1 = start_ptr;
		node *cursor2 = new node(cursor1->data());
		node * head = cursor2;

		// loop from start until finish and copy nodes
		for (size_t i = start; i < finish; ++i)
		{
			// move cursor1, set link, move cursor 2
			cursor1 = cursor1->link();
			cursor2->set_link(new node(cursor1->data()));
			cursor2 = cursor2->link();
		}

		return head;
	}

    void list_remove_dups(node* head_ptr)
	{
		// check list is non-empty
		if (head_ptr != NULL)
		{
			// set up node pointers for compare node, traversal node, and node to remove
			node *compare_ptr = head_ptr;
			node *cursor, *remove_ptr;

			// loop while there are still nodes to check
			while (compare_ptr->link() != NULL)
			{
				// set cursor to current node to check
				cursor = compare_ptr;

				// loop while not at end of list
				while (cursor->link() != NULL) 
				{
					// next pointer in list's data equals compare data remove, else move cursor
					if (compare_ptr->data() == cursor->link()->data())
					{
						remove_ptr = cursor->link();
						cursor->set_link(remove_ptr->link());
						delete remove_ptr;
						continue;
					}
					cursor = cursor->link();
				}
			}
		}
	}

    void list_print (const node* head_ptr)
	{
		// if list empty
		if (head_ptr == NULL) 
		{
			std::cout << std::endl;	
			return;
		}

		// loop through each node and print data
		std::cout << head_ptr->data();
		head_ptr = head_ptr->link();
		while (head_ptr != NULL)
		{
			std::cout << ", " << head_ptr->data();
			head_ptr = head_ptr->link();
		}
		std::cout << std::endl;
	}

    node* list_detect_loop (node* head_ptr)
	{
		// set up slow and fast node pointers (traverse list)
		node *slow = head_ptr;
    	node *fast = head_ptr;

		// while fast and slow are not at the same node
    	while (fast != NULL && fast->link() != NULL) 
		{
			// update fast and slow pointers (slow + 1, fast + 2)
        	slow = slow->link();
        	fast = fast->link()->link();
			if (slow == fast) break;
    	}

		// fast reached end of list -> no loop
		if (fast == NULL || fast->link() == NULL) return NULL;

		// increment slow and fast by 1 node each until they meet
		slow = head_ptr;
    	while (slow != fast) 
		{
        	slow = slow->link();
        	fast = fast->link();
    	}

    	return fast;
	}
}
