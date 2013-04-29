namespace utils { 
#include <cmath>
#include <iostream>
typedef struct Click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
    float norma() {
        return sqrt(x*x + y*y);
    }
    float normaManhattan() {
        return x+y;
    }
}Click;
}
