#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
using namespace cimg_library;   //Necesario

//@ Lee los parches donde aplicar la ecualizacion
std::vector< CImg<unsigned char> > get_parches(std::string nombre, CImg<unsigned char> imagen, std::vector<std::vector<unsigned int> > &coordenadas) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }

    coordenadas.clear();

    unsigned int x0;
    unsigned int y0;
    unsigned int x1;
    unsigned int y1;

    std::vector<CImg<unsigned char> > salida;

    while(f>>x0>>y0>>x1>>y1) {
        std::vector<unsigned int> tempy;
        CImg<unsigned char> parche = imagen.get_crop(x0, y0, x1, y1); 
        salida.push_back(parche);
        tempy.push_back(x0);
        tempy.push_back(y0);
        tempy.push_back(x1);
        tempy.push_back(y1);
        coordenadas.push_back(tempy);

    }
    f.close();
    return salida;
}

using namespace std;

int main(int argc, char *argv[]) {
    //@ Encontrar cosas oscuras mediante ecualizacion de histograma 

    const char* _inputA = "../images/cuadros.tif";

    CImg<unsigned char> input(_inputA), output;

    output = input;

    std::vector<std::vector<unsigned int> > coordenadas;
    std::vector<CImg<unsigned char> > parches;

    //Lee los parches donde aplicar la ecualizacion
    parches = get_parches("mascaras_ej72.txt", input, coordenadas);

    unsigned int n = coordenadas.size();

    for (unsigned int i = 0; i < n; i++) {
        //Ecualiza el parche
        parches[i].equalize(256);
        //Reemplaza los valores del parche
        for (unsigned int j=coordenadas[i][0], x = 0; j <  coordenadas[i][2]; j++, x++ ) {
            for (unsigned int k=coordenadas[i][1], y = 0; k <  coordenadas[i][3]; k++, y++ ) {
                output(j,k) =  parches[i](x,y);
            }
        }

    }

    //Dibuja
    CImgList<unsigned char> lista;
    lista.assign(input, output);
    lista.display();
}


