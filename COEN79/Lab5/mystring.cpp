#include "mystring.h"
#include <cstring>
#include <math.h>

namespace coen79_lab5 {
    // CONSTRUCTOR for the string class:
    string::string(const char str[ ]) {
        /* Precondition: str is an ordinary null-terminated string.
        Postcondition: The string contains the sequence of chars from str. */
        current_length = std::strlen(str);
        allocated = current_length + 1;
        characters = new char[allocated];
        std::strncpy(characters, str, current_length);
        characters[length()] = '\0';
    }

    string::string(char c) {
        /* Postcondition: The string contains c and then the null character. */
        characters = new char[2];
        characters[0] = c;
        characters[1] = '\0';
        allocated = 2;
        current_length = 1;
    }

    string::string(const string& source) {
        /* Postcondition: The string becomes a copy of the source string. */
        current_length = source.length();
        allocated = current_length + 1;
        characters = new char[allocated];
        std::strncpy(characters, source.characters, current_length);
        characters[length()] = '\0';
    }

    string::~string( ) {
        delete[ ] characters;
        allocated = 0;
        current_length = 0;
    }

    
    // MODIFICATION MEMBER FUNCTIONS for the string class:
    void string::operator +=(const string& addend) {
        /* Postcondition: addend has been catenated to the end of the string. */
        size_t new_length = length() + addend.length(), cur_length = length();
        if (new_length >= current_length) reserve(new_length + 1);
        for (int i = cur_length; i < new_length; ++i) {
            characters[i] = addend[i - cur_length];
        }
        current_length = new_length;
    }

    void string::operator +=(const char addend[ ]) {
        /* Precondition: addend is an ordinary null-terminated string.
        Postcondition: addend has been catenated to the end of the string. */
        // ***
        size_t new_length = length() + std::strlen(addend), cur_length = length();
        if (new_length >= current_length) reserve(new_length + 1);
        for (int i = cur_length; i < new_length; ++i) {
            characters[i] = addend[i - cur_length];
        }
        current_length = new_length;
    }

    void string::operator +=(char addend) {
        /* Postcondition: The single character addend has been catenated to the
        end of the string. */
        if (current_length + 1 >= allocated) reserve(allocated + 1);
        characters[current_length] = addend;
        ++current_length;
    }

    void string::reserve(size_t n) {
        /* Postcondition: All functions will now work efficiently (without
        allocating new memory) until n characters are in the string. */
        char *new_chars;
        if (n < length()) n = length();
        new_chars = new char[n];
        strncpy(new_chars, characters, n);
        delete[] characters;
        characters = new_chars;
        allocated = n;
    }

    string& string::operator =(const string& source) {
        /* Postcondition: returned string is a copy of the source string */
        unsigned int src_len = source.length();
        char *copy = new char[src_len + 1];
        strncpy(copy, source.characters, src_len);
        delete[] characters;
        characters = copy;
        allocated = src_len + 1;
        current_length = src_len;
        return *this;
    }

    void string::insert(const string& source, unsigned int position) {
        /* Precondtion: position <= length()
        Postcondition: The source string is inserted into this string before
        the character at the given index. */
        assert(position <= length());
        // get pointer to remaining characters after position
        char *rem = new char[length() - position];
        std::strncpy(rem, characters + position, length() - position);
        // allocate more space for addition of source if needed
        unsigned int new_len = length() + source.length();
        if (new_len >= allocated) reserve(new_len + 1);
        // copy source into position
        std::strncpy(characters + position, source.characters, source.length());
        // copy remaining characters into array after source
        std::strncpy(characters + position + 1, rem, length() - position);
        current_length = new_len;
        characters[current_length] = '\0';
    }

    void string::dlt(unsigned int position, unsigned int num) {
        /* Precondition: (position + num) <= length( ).
        Postcondition: num characters are deleted from the sequence, starting
        at index position. */
        assert((position + num) <= length());
        // get pointer to characters after delete range
        char *sub = characters + position + num;
        // copy characters after delete range to position then decrement length
        std::strncpy(characters + position, sub, length() - position - num);
        current_length -= num;
        characters[length()] = '\0';
    }

    void string::replace(char c, unsigned int position) {
        /* Precondition: position < length()
        Postcondtion: the character at the given position in the string is
        replaced with c. */
        assert(position < length());
        characters[position] = c;
    }

    void string::replace(const string& source, unsigned int position) {
        /* Precondition: (position + source.length()) <= length()
        Postcondtion: the characters in this string starting at position are
        replaced with those in the source string. */
        assert((position + source.length()) <= length());
        for (int i = position; i < position + source.length(); ++i) {
            characters[i] = source.characters[i - position];
        }
    }

    
    // CONSTANT MEMBER FUNCTIONS for the string class:
    char string::operator [ ](size_t position) const {
        /* Precondition: position < length( ).
        Postcondition: The value returned is the character at the specified
        position of the string. A string's positions start from 0 at the start
        of the sequence and go up to length( )-1 at the right end. */
        assert(position < length());
        return characters[position];
    }

    int string::search(char c) const {
        /* Postcondition: The location of the first occurence of the character c
        within this string is returned. If the string does not contain c, -1 is
        is returned. */
        for (int i = 0; i < length(); ++i) {
            if (characters[i] == c) return i;
        }
        return -1;
    }

    int string::search(const string& substring) const {
        /* Postcondition: Returns the index of the start of the first instance of
        the given substring inside of this string. If the substring is not found,
        this function returns -1. */
        int fc = search(substring.characters[0]), j = 0;
        if (fc == -1) return -1;
        for (int i = fc; i < length() - substring.length(); ++i) {
            while (characters[i] == substring.characters[j]) {
                if (j == substring.length() - 1) return i;
                ++j;
            }
            i += j;
            j = 0;
        }
        return fc;
    }

    unsigned int string::count(char c) const {
        /* Postcondition: The count of the occurence of the character c within this
        string is returned. */
        unsigned int i = 0, cnum = 0;
        while (i < length()) {
            if (characters[i] == c) ++cnum;
            ++i;
        }
        return cnum;
    }


    // FRIEND FUNCTIONS
    std::ostream& operator <<(std::ostream& outs, const string& source) {
        /* Postcondition: The sequence of characters in source has been written
        to outs. The return value is the ostream outs. */
        for (int i = 0; i < source.length(); ++i) {
            outs << source.characters[i];
        }
        return outs;
    }

    bool operator ==(const string& s1, const string& s2) {
        if (s1.length() != s2.length()) return false;
        bool res = true;
        size_t i = 0;
        // while the characters are the same continue
        while (res == true) {
            if (i >= s1.length() || i >= s2.length()) break;
            // if they are diff set to false and break
            if (s1[i] != s2[i]) res = false;
            ++i;
        }
        return res;
    }

    bool operator !=(const string& s1, const string& s2) {
        if (s1.length() != s2.length()) return true;
        bool res = true;
        size_t i = 0;
        // opposite of operator==
        while (res == true) {
            if (i >= s1.length() || i >= s2.length()) break;
            if (s1[i] == s2[i]) res = false;
            ++i;
        }
        return res;
    }

    bool operator >(const string& s1, const string& s2) {
        if (s1.length() < s2.length() || s1 == s2) return false;
        bool res = true;
        size_t i = 0;
        // while s1 char is greater than s2 char continue
        while (res == true) {
            if (i >= s1.length() || i >= s2.length()) break;
            if (s1[i] < s2[i]) res = false;
            ++i;
        }
        return res;
    }

    bool operator <(const string& s1, const string& s2) {
        if (s1.length() > s2.length() || s1 == s2) return false;
        bool res = true;
        size_t i = 0;
        // opposite of operator>
        while (res == true) {
            if (i >= s1.length() || i >= s2.length()) break;
            if (s1[i] > s2[i]) res = false;
            ++i;
        }
        return res;
    }

    bool operator >=(const string& s1, const string& s2) {
        // use operator> and operator==
        if (s1 > s2 || s1 == s2) return true;
        return false;
    }

    bool operator <=(const string& s1, const string& s2) {
        // operator< and operator==
        if (s1 < s2 || s1 == s2) return true;
        return false;
    }

    
    // NON-MEMBER FUNCTIONS for the string class:
    string operator +(const string& s1, const string& s2) {
        /* Postcondition: The string returned is the catenation of s1 and s2. */
        string str(s1);
        str += s2;
        return str;
    }

    string operator +(const string& s1, const char addend[ ]) {
        /* Postcondition: The string returned is the catenation of s1 and s2. */
        string str(s1);
        str += addend;
        return str;
    }

    std::istream& operator >>(std::istream& ins, string& target) {
        /* Postcondition: A string has been read from the istream ins, and the
        istream ins is then returned by the function. The reading operation
        skips white space (i.e., blanks, newlines, tabs) at the start of ins.
        Then the string is read up to the next white space or the end of the
        file. The white space character that terminates the string has not
        been read. */
        int i = 0;
        char c;
        // clear leading whitespace
        ins >> std::ws;
        // while c is valid character
        while ((c = ins.get()) > 0) {
            // break once whitespace is read
            if (isspace(c)) break;
            // set target char to c
            target.characters[i] = c;
            ++i;
        }
        target.current_length = i;
        target.characters[target.length()] = '\0';
        target.allocated = i + 1;
        return ins;
    }
}