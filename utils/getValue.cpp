namespace utils{

//Devuelve un valor acotado a 0~255
unsigned int getValue(unsigned int x, unsigned int inicio, float pendiente, float offset) {
    float value = (x - inicio) * pendiente + offset;
    if (value > 255) 
        return 255;
    else if (value < 0)
        return 0;
    else 
        return (unsigned int) value;
}

}
