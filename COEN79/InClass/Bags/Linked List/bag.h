#include "node.h"
#include "lltk.h"
#include <cstdlib>

// similar to std::forward_list

class bag {
    public:
        typedef node::value_type value_type;
        typedef size_t size_type;
        bag() { head = nullptr; size = 0; }
        ~bag() { listClear(head); }
        bag(const bag& src) { 
            node* tail;
            listCopy(src.head, head, tail); // on exam implement this function
            size = src.size;
        }
        void operator=(const bag& src) { // exam question
            if (this == &src) return;
            listClear(head);
            node* tail;
            listCopy(src.head, head, tail);
        }
        void operator==(const bag& l, const bag& r) { // exam question
            
        }

    private:
        node* head;
        size_type size;
}