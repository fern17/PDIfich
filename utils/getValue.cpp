namespace utils{
#include <cmath>

unsigned int truncate(float value) {
    if (value > 255) 
        return 255;
    else if (value < 0)
        return 0;
    else 
        return floor(value);
}


//Devuelve un valor acotado a 0~255
unsigned int getLinearValue(unsigned int x, unsigned int inicio, float pendiente, float offset) {
    float value = (x - inicio) * pendiente + offset;
    return truncate(value);
}

unsigned int getLogarithmicValue(unsigned int x, float c=1.0) {
    float value = c*log(1+x);
    float ret_val = truncate(value);
    return ret_val;
}

unsigned int getPotValue(unsigned int x, float c, float gamma) {
    float value = c*pow(x,gamma);
    float ret_val = truncate(value);
    return ret_val;
}
}
