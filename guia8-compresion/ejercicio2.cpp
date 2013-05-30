#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <fstream>
using namespace cimg_library;   //Necesario


std::vector<unsigned int> getRLC(CImg<bool> & imgbinaria) {
    bool contando_el = 0;
    unsigned int contador = 0;

    std::vector<unsigned int> retval;

    for (unsigned int x = 0; x < imgbinaria.height(); x++ ) {
        for (unsigned int y = 0; y < imgbinaria.width(); y++ ) {
            
            if (imgbinaria(x,y) != contando_el) {
                contando_el = !contando_el;
                retval.push_back(contador);
                contador = 1;
            } else {
                contador++;
            }
        }
    }
    return retval;
}

CImg<bool> getRLCInverse(std::vector<unsigned int> & v) {
    bool contando_el = v[0];
    unsigned int contador = 0;
    unsigned int w = v[1];
    unsigned int h = v[2];

    CImg<bool> retval(w,h);
    unsigned int xx = 0;
    unsigned int yy = 0;

    for (unsigned int i = 3; i < v.size(); i++) {
        int faltan = v[i];
        if (faltan == 0) {
            contando_el = !contando_el;
            continue;
        } 
        //std::cout<<faltan<<' '<<contando_el<<'\n'; std::getchar();
        for (unsigned int x = xx; x < w && faltan > 0; x++) {
            for (unsigned int y = yy; y < h && faltan > 0; y++) {
        		retval(x,y) = contando_el;
        		faltan--;
                xx++;
                yy++; //this is wrong
                xx = xx % w;
                yy = yy % h; //this too
        		if (faltan == 0) {
        		    contando_el = !contando_el;
        		    //faltan = 0; //para evitar qeu entre en el otro if
        		}                
            }
        }
    }
    
    return retval;
}

void printVector(std::vector<unsigned int> v) {
    std::cout<<"Vector size = "<<v.size()<<'\n';
    for (unsigned int i = 0; i < v.size(); i++)
        std::cout<<v[i]<<' ';
    std::cout<<'\n';
}

int main(int argc, char *argv[]) {
    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const unsigned int _umbral = cimg_option("-u", 100.0, "Input Image File");


    CImg<double> imagen(_input);
    CImg<bool> imagen_binaria(imagen.width(), imagen.height(), 1, 1, 0);

    
    cimg_forXY(imagen, x, y) {
        if (imagen(x,y) > _umbral)
            imagen_binaria(x,y) = true;
    }


    
    std::vector<unsigned int> rlc = getRLC(imagen_binaria);
    rlc.insert(rlc.begin(), imagen_binaria.height() ); //alto
    rlc.insert(rlc.begin(), imagen_binaria.width() ); //ancho
    rlc.insert(rlc.begin(), 0 ); //comienza con


    //Guardamos
    std::ofstream file("ej2-rlc.jpg", std::ios::binary | std::ios::trunc | std::ios::out );

    assert(file.is_open());

    file.write(reinterpret_cast<char *>(& rlc[0]), rlc.size()*sizeof(rlc[0]) );
    //file.write(&rlc[0], rlc.size());
    file.close();

    //printVector(rlc);

    //Leemos
    std::ifstream f("ej2-rlc.jpg", std::ios::binary | std::ios::in );
    f.seekg(0, std::ifstream::end);
    unsigned int tamano = f.tellg();
    f.seekg(0, std::ifstream::beg);

    std::vector<unsigned int> lectura;
    lectura.resize(tamano/(sizeof(unsigned int)));
    
    //f.read(reinterpret_cast<char *> (& lectura[0]), tamano);
    f.read((char *) &lectura.front(), lectura.size()*sizeof(lectura.front()));
    f.close();
    
    printVector(lectura);
    
    CImg<bool> img_leida;
    img_leida = getRLCInverse(lectura);
    
    CImg<unsigned char> img_leida2(img_leida.width(), img_leida.height(),1,1,0);
    cimg_forXY(img_leida,x,y) {
        if (img_leida(x,y))
            img_leida2(x,y) = 255;
    }
    img_leida2.display();

    return 0;
}
