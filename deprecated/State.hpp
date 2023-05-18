#include <iostream>

class State {
public:
    State(bool accepting) : _accepting{accepting} {};
    bool isAccepting() { return _accepting; }
    //void print();
private:
    bool _accepting;

};