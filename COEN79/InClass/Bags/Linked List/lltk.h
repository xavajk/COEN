#include "node.h"

void listHeadInsert (node*& head, const node::value_type& item) {
    head = new node(item, head);
}

void listHeadRemove(node*& head_ptr) {
    node* tmp = head_ptr;
    head_ptr = head_ptr->link();
    delete tmp;
}

void listClear(node*& head_ptr) {
    while (head_ptr != nullptr) {
        listHeadRemove(head_ptr);
    }
}

void listCopy(const node* src_ptr, node*& head_ptr, node*& tail_ptr) {
    head_ptr = nullptr, tail_ptr = nullptr;
    if (src_ptr == nullptr) return;
    // add first node to list copy and assign head/tail
    head_ptr = new node(src_ptr->data(), nullptr);
    tail_ptr = head_ptr;
    // loop through original list and add rest of remaining nodes
    while (src_ptr->link() != nullptr) {
        src_ptr = src_ptr->link();
        tail_ptr->link() = new node(src_ptr->data(), nullptr);
        tail_ptr = tail_ptr->link();
    }
}

node* listSearch(node* head_ptr, const node::value_type& target) {
    node* cursor = head_ptr;
    while (cursor != nullptr) {
        // requires overload of operator== for value_type
        if (cursor->data() == target) return cursor;
        cursor = cursor->link();
    }
    return nullptr;
}

const node* listSearch(const node* head_ptr, const node::value_type& target) {
    const node* cursor = head_ptr;
    while (cursor != nullptr) {
        // requires overload of operator== for value_type
        if (cursor->data() == target) return cursor;
        cursor = cursor->link();
    }
    return nullptr;
}

// Floyd's loop detection algorithm (avoid infinte loops when traversing)
/*  requires 2 pointers, slow runner and fast runner, who both initially point to the head. 
    slow runner (s) moves one step and fast runner (f) moves two steps. once they meet the
    first time, make s point to the head and reduce the speed of f to 1 step. they will then
    meet again at the beginning of the loop. f (link) becomes null if there is no loop.  */

return_type loopDetect(const node* head) {
    node* s = head;
    node* f = head;
    while (s != f) {
        if (f == nullptr) return _____; // no loop
        s = s->link();
        f = f->link()->link();
    }
    s = head;
    while (s != f) {
        s = s->link();
        f = f->link();
    }
    return return_type;
}
