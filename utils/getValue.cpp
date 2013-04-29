namespace utils{
#include <cmath>

unsigned char truncateValue(float value) {
    if (value > 255) 
        return 255;
    else if (value < 0)
        return 0;
    else 
        return floor(value);
}


//Devuelve un valor acotado a 0~255
unsigned char getLinearValue(unsigned int x, unsigned int inicio, float pendiente, float offset) {
    float value = (x - inicio) * pendiente + offset;
    return truncateValue(value);
}

unsigned char getLogarithmicValue(unsigned int x, float c=1.0) {
    float value = c*log(1+x);
    float ret_val = truncateValue(value);
    return ret_val;
}

unsigned char getPotValue(unsigned int x, float c, float gamma) {
    float value = c*pow(x,gamma);
    float ret_val = truncateValue(value);
    return ret_val;
}
}
