// FILE: company.cpp
// CLASS implemented: company (see company.h for documentation)


#include <cassert>
#include "company.h"

//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif

namespace coen79_lab7
{
    company::company() {
        this->company_name = "";
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }
    
    company::company(const std::string& company_name) {
        assert(company_name.length() > 0);
        this->company_name = company_name;
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }
    
    company::company(const company &src) {
        Debug("Company copy constructor..." << std::endl);
        this->company_name = src.company_name;
        node *new_head, *new_tail;
        list_copy(src.head_ptr, new_head, new_tail);
        this->head_ptr = new_head;
        this->tail_ptr = new_tail;
    }

    
    company& company::operator= (const company &src) {
        Debug("Company assignemnt operator..." << std::endl);
        this->company_name = src.company_name;
        node *new_head, *new_tail;
        list_copy(src.head_ptr, new_head, new_tail);
        this->head_ptr = new_head;
        this->tail_ptr = new_tail;
        return *this;
    }
    
    
    company::~company() {
        list_clear(head_ptr);
    }
    
    
    std::string company::get_name() const {
        return company_name;
    }
    
    const node *company::get_head() const {
        return head_ptr;
    }
    
    const node *company::get_tail() const {
        return tail_ptr;
    }
    
    node* company::get_head() {
        return head_ptr;
    }
    
    node* company::get_tail() {
        return tail_ptr;
    }
    
    void company::print_items() {
        list_print(head_ptr);
    }
    
    
    bool company::insert(const std::string& product_name, const float& price) {
        
        assert(product_name.length() > 0);

        // if the product is already in the list
        if (list_contains_item(head_ptr, product_name)) {
            return false;
        }
        
        // if the list is empty
        if (head_ptr == NULL) {
            head_ptr = new node(product_name, price, NULL);
            tail_ptr = head_ptr;
        }

        // otherwise, insert at back
        else {
            list_tail_insert(tail_ptr, product_name, price);
        }
        
        return true;
    }

    
    bool company::erase(const std::string& product_name) {
        assert(product_name.length() > 0);
        node *cursor = head_ptr, *precursor;

        // check that product is in list
        if (list_contains_item(head_ptr, product_name) == false) return false;

        // check if head is target item
        if (cursor->getName() == product_name) list_head_remove(head_ptr);

        // otherwise loop to find desired item
        else {
            precursor = cursor;
            cursor = cursor->getLink();
            while (cursor->getName() != product_name) {
                if (cursor == tail_ptr) break;
                precursor = cursor;
                cursor = cursor->getLink();
            }

            // at desired node, remove
            precursor->setLink(cursor->getLink());
            if (cursor == tail_ptr) tail_ptr = precursor;
            delete cursor;
        }

        return true;
    }
}
