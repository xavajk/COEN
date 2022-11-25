#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;
int main(int argc, char const *argv[]) {
    // ensure proper usage
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <filename>." << endl;
        exit(1);
    }

    // read content from file
    ifstream read_file(argv[1]);
    string text;

    // iterate through each word in file
    while (read_file >> text) {
        string upper;
        // check that char is alphanum and change char to uppercase
        for (int i = 0; i < text.length(); ++i) {
            if (isalpha(text[i]) or isnumber(text[i])) {
                upper += toupper(text[i]);
            }
        }
        // if uppercase word length > 9 print
        if (upper.length() > 9) {
            cout << upper << endl;
        }
    }
    read_file.close();

    return 0;
}
