// FILE: database.cpp
// CLASS implemented: database (see database.h for documentation)

// INVARIANT for the database ADT:
//   1. The items in the database are stored in a dynamic array, where each entry of the array includes
//      a company name, a pointer to the head of the linked list of products, and a
//      a pointer to the tail of the linked list of products
//   2. The number of slots in the array of companies is stored in member varibale aloc_slots
//   3. The number of used slots of the array of companies is stored in member varibale used_slots


#ifndef DATABASE_CPP
#define DATABASE_CPP

#include "database.h"


//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif


namespace coen79_lab7
{
    
    database::database(const size_type& initial_capacity) {
        used_slots = 0;
        aloc_slots = initial_capacity;
        company_array = new company[aloc_slots];
    }
    
    
    database::database(const database &src) {
        Debug("Copy constructor..." << std::endl);

        // update member variables
        used_slots = src.used_slots;
        aloc_slots = src.aloc_slots;

        // allocated company array and set entries equal to source
        company_array = new company[aloc_slots];
        std::copy(src.company_array, src.company_array + used_slots, company_array);
    }
    
    
    database& database::operator= (const database &rhs) {
        Debug("Assignment operator..." << std::endl);

        // check for self-assignment
        if (this == &rhs) return *this;

        // update member variables
        used_slots = rhs.used_slots;
        aloc_slots = rhs.aloc_slots;

        // create new company array and set entries equal to source
        company *new_array = new company[aloc_slots];
        std::copy(rhs.company_array, rhs.company_array + used_slots, new_array);

        // delete old company array and set equal to new
        delete[] company_array; 
        company_array = new_array;
        return *this;
    }
    
    
    database::~database() {
        delete[] company_array;
    }
    
    
    void database::reserve(size_type new_capacity) {
        Debug("Reserve..." << std::endl);

        if (new_capacity == aloc_slots)
            return; // The allocated memory is already the right size.
        
        if (new_capacity < used_slots)
            new_capacity = used_slots; // Cannot allocate less than we are using.
        
        // create new array and copy items from old array
        company *new_array = new company[new_capacity];
        std::copy(company_array, company_array + used_slots, new_array);

        // delete old company array and set equal to new
        delete[] company_array; 
        company_array = new_array;
        aloc_slots = new_capacity;
    }
    
    
    bool database::insert_company(const std::string &entry) {
        Debug("Insert company..." << std::endl);
        assert(entry.length() > 0);
        size_type pos = search_company(entry);
        
        // if you find a company that is false, because there are duplicates
        if (pos != COMPANY_NOT_FOUND) {
            return false;
        }

        // check if need to reserve more space
        if (used_slots == aloc_slots) reserve(aloc_slots + 1);

        // insert company at end of array
        company_array[used_slots++] = company(entry);

        return true;
    }

    
    bool database::insert_item(const std::string &company, const std::string &product_name, const float &price) {
        Debug("Insert item..." << std::endl);
        assert(company.length() > 0 && product_name.length() > 0);

        // ensure the given company exists in the array
        int pos = search_company(company);
        if (pos == COMPANY_NOT_FOUND) return false;

        // insert product into given company
        company_array[pos].insert(product_name, price);

        return true;
    }
    
    
    bool database::erase_company(const std::string &company) {
        Debug("Erase company..." << std::endl);
        assert(company.length() > 0);
        size_type company_index = search_company(company);
        
        // ensure that the given company exists in the list
        if (company_index == COMPANY_NOT_FOUND) return false;

        // erase company and shift elements
        for (int i = company_index; i < used_slots - 1; ++i) {
            company_array[i] = company_array[i + 1];
        }
        --used_slots;
        return true;
    }
    
    
    bool database::erase_item(const std::string& cName, const std::string& pName) {        
        assert(cName.length() > 0 && pName.length() > 0);

        // check that the company exists
        int company_index = search_company(cName);
        if (company_index == COMPANY_NOT_FOUND) return false;

        // check if item was erased or not
        bool item_erased = company_array[company_index].erase(pName);

        return item_erased;
    }
    
    
    database::size_type database::search_company(const std::string& company) {
        assert(company.length() > 0);

        // loop through database to search for given company and return index if found
        for (int i = 0; i < used_slots; ++i) {
            if (company.compare(company_array[i].get_name()) == 0) return i;
        }

        return COMPANY_NOT_FOUND;
    }
    
    
    bool database::print_items_by_company(const std::string& cName) {
        assert(cName.length() > 0);

        size_type c_index = search_company(cName);
        
        if (c_index == COMPANY_NOT_FOUND) {
            return false;
        }
        
        std::cout << "Printing the products of " << cName << ":"<< std::endl;
        company_array[c_index].print_items();
        std::cout << std::endl;
        
        return true;
    }
    
    
    void database::print_companies() {
        std::cout << "Company List" << std::endl;
        for (int i = 0; i < used_slots; i++) {
            std::cout << "- " << company_array[i].get_name() << std::endl;
        }
    }
}

#endif
