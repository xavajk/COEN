#include <stdlib.h>
#include <iostream>

using namespace std;
int main(int argc, char const *argv[]) {   
    // get input from user
    cout << "Please input a string: ";
    string in;
    int num_alpha = 0;
    int num_other = 0;

    // read from input stream
    while(getline(cin, in)) {
        // loop through input to tally characters
        for (int i = 0; i < in.length(); ++i) {
            if (isalpha(in[i]) or isnumber(in[i])) num_alpha++;
            else if (in[i] != 32) num_other++;
        }
        break;
    }

    // print results to output
    cout << "The number of alphanumeric characters is " << num_alpha << "." << endl;
    cout << "The number of non-alphanumeric characters is " << num_other << "." << endl;
    return 0;
}
