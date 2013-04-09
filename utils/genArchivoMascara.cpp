#include <iostream>
#include <fstream>

void genArchivoMascara(std::string archivo, unsigned int ancho, unsigned int alto) {
    std::ofstream file(archivo.c_str(), std::ios::trunc);
    file<<alto<<ancho;
    for (unsigned int i = 0; i < alto; i++) {
        for (unsigned int j = 0; j < ancho; j++) {
            float value = 1.0/(ancho*alto);
            file<<value;
            cout<<(float)value<<' ';
        }
        cout<<'\n';
    }
    file.close();
}
