#include <iomanip>
#include <iostream>

using namespace std;
int main(int argc, char const *argv[]) {
    string str = "0123456789";
    string str_reverse = "9876543210";

    int width = str.length();
    for (int i = 0; i < 10; i += 2) {
        cout << setw(width + i)
             << str
             << setw(2*width + i)
             << str_reverse << endl;
    }

    return 0;
}
