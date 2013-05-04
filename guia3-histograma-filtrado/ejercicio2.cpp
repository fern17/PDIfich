#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace cimg_library;   //Necesario
using namespace std;

//@ Lee un archivo con las coordenadas de los parches a obtener la imagen parametro.
//  Devuelve un vector con imagenes que son los parches de la imagen original
std::vector< CImg<unsigned char> > get_parches(std::string nombre, CImg<unsigned char> imagen) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int x0;
    unsigned int y0;
    unsigned int x1;
    unsigned int y1;

    std::vector<CImg<unsigned char> > salida;

    while(f>>x0>>y0>>x1>>y1) {
        CImg<unsigned char> parche = imagen.get_crop(x0, y0, x1, y1); 
        salida.push_back(parche);
    }
    f.close();
    return salida;
}


int main(int argc, char *argv[]) {
    //@ Identificar de ruido a partir de histograma, leer desde archivo separado por espacios

    const char* _inputA = "../images/FAMILIA_a.jpg";
    const char* _inputB = "../images/FAMILIA_b.jpg";
    const char* _inputC = "../images/FAMILIA_c.jpg";

    CImg<unsigned char> imgA(_inputA),
                        imgB(_inputB),
                        imgC(_inputC);

    std::vector<CImg<unsigned char> > imagenes; 
    imagenes.push_back(imgA);
    imagenes.push_back(imgB);
    imagenes.push_back(imgC);

    unsigned int n = imagenes.size();

    for (unsigned int i = 0; i < n; i++) {
        cout<<"Procesando imagen: "<<i+1<<endl;
        
        vector<CImg<unsigned char> > parches;
        parches = get_parches("parches_ruido.txt", imagenes[i]);

        unsigned int m = parches.size();
        for (unsigned int j = 0; j < m; j++) {
            cout<<"\tProcesando parche: "<<j+1;
            cout<<"\tMedia: "<<parches[j].mean()<<"\tVarianza: "<<parches[j].variance()<<endl;
            parches[j].get_histogram(256).display_graph(0,3);
        }
    }
}


//  imagen 1
//  parche 1 -> gaussiano
//  parche 2 -> no se aprecia
//  parche 3 -> gaussiano
//    -> gaussiano
// imagen 2
//  parche 1 -> uniforme
//  parche 2 -> no se aprecia
//  parche 3 -> uniforme 
//    -> uniforme
// imagen 3
//  parche 1 -> sal pimienta
//  parche 2 -> sal pimienta
//  parche 3 -> sal pimienta
//    -> sal y pimienta
