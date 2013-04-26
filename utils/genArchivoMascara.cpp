#include <iostream>
#include <fstream>

namespace utils {
//Genera una mascara de promediado
void genArchivoMascara(std::string archivo, unsigned int ancho, unsigned int alto) {
    std::ofstream file(archivo.c_str(), std::ios::trunc);
    file<<alto<<' '<<ancho<<'\n';
    for (unsigned int i = 0; i < alto; i++) {
        for (unsigned int j = 0; j < ancho; j++) {
            float value = 1.0/(ancho*alto);
            file<<value<<' ';
            // cout<<(float)value<<' ';
        }
        file<<'\n';
    }
    file.close();
}

//genera una mascara gaussiana
void genArchivoMascaraGaussiana(std::string archivo, unsigned int ancho, unsigned int alto, float sigma) {
    std::ofstream file(archivo.c_str(), std::ios::trunc);
    file<<alto<<' '<<ancho<<'\n';
    for (unsigned int i = 0; i < alto; i++) {
        for (unsigned int j = 0; j < ancho; j++) {
            int xx = j - (ancho/2.0);
            int yy = i - (alto/2.0);
            float value = (1/(2*3.14*sigma)) * exp(-(xx*xx+yy*yy)/(2*sigma*sigma));
            file<<value<<' ';
            //std::cout<<(float)value<<' ';
        }
        file<<'\n';
        //std::cout<<"\n";
    }
    file.close();
}

}
