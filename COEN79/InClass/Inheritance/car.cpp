#include <iostream>
#include <string>

using namespace std;

// car class becomes abstract b/c of the pure virtual function
class car {
    public:
    string getMake() const { return make; }

    protected:
    virtual void accelerate() = 0;  // pure virtual function

    private:
    string make;
    string model;
    string color;
    int speed;
};

class electric : public car {
    public:
    void accelerate() {
        // update speed
        // decrease battery capacity
    }

    private:
    int battery_cap;
}