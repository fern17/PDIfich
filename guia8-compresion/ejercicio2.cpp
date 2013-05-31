#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <fstream>
#include <string>
using namespace cimg_library;   //Necesario


//@ Realiza la codificacion RLC a partir de una imagen binaria
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

//@ Realiza la descodificacion RLC a partir de un vector de repeticiones
//Se asume que el primer valor es el valor inicial
//El segundo valor es el ancho
//El tercer valor es el alto
//A partir del 4to estan las repeticiones, iniciando con el valor inicial especificado
CImg<bool> getRLCInverse(std::vector<unsigned int> & v) {
    bool contando_el = v[0];
    unsigned int contador = 0;
    unsigned int w = v[1];
    unsigned int h = v[2];

    CImg<bool> retval(w,h);

    unsigned int pos = 3;
    int restantes = v[pos];
    cimg_forXY(retval,x,y) {
        if (restantes == 0) {
            pos++;
            if (pos >= v.size()) {
                break;
                //std::cout<<"Error en la compresion, fuera de rango\n";
                return retval;
            }
            else 
                restantes = v[pos]; //nuevo contador
            contando_el = !contando_el; //invertimos
        }
        retval(y,x) = contando_el;
        restantes--;
    }
    return retval;
}

CImg<bool> binarizar(CImg<unsigned int> imagen, unsigned int umbral) {
    CImg<bool> imagen_binaria(imagen.width(), imagen.height(), 1, 1, 0);
    
    //Binarizamos la imagen
    cimg_forXY(imagen, x, y) {
        if (imagen(x,y) > umbral)
            imagen_binaria(x,y) = true;
    }
    return imagen_binaria;
}

//@ Comprime una imagen escala de gris a RLC con el nombre que se le pasa, el umbral de binarizacion especificado y el valor de inicio dado
CImg<bool> RLCCompression(CImg<double> img, unsigned int umbral, std::string filename, bool inicio = false) {

    //Binarizamos
    CImg<bool> imagen_binaria = binarizar(img, umbral);

    //Obtenemos el vector a comprimir
    std::vector<unsigned int> rlc = getRLC(imagen_binaria);
    //Insertamos: [inicio, ancho, alto, ...] (se insertan al reves)
    rlc.insert(rlc.begin(), imagen_binaria.height() ); //alto
    rlc.insert(rlc.begin(), imagen_binaria.width() ); //ancho
    rlc.insert(rlc.begin(), 0 ); //comienza con

    //Guardamos el vector de compresion
    //Abre el archivo
    std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc | std::ios::out );
    //Patea si no esta abierto
    assert(file.is_open());
    //Escribe el vector entero
    file.write(reinterpret_cast<char *>(& rlc[0]), rlc.size()*sizeof(rlc[0]) );
    //Cerramos
    file.close();
    return imagen_binaria;
}

//@ Realiza todos los pasos para descomprimir una imagen a partir de un archivo con codificacion RLC
CImg<bool> RLCDecompression(std::string filename) {
    //Ahora leemos el archivo comprimido y tratamos de descomprimirlo
    //
    //Leemos
    std::ifstream f(filename.c_str(), std::ios::binary | std::ios::in );
    
    //Movemos el puntero al final para saber cuanto tamaño tiene
    f.seekg(0, std::ifstream::end);
    unsigned int tamano = f.tellg();
    //Lo volvemos al principio para hacer una lectura secuencial
    f.seekg(0, std::ifstream::beg);

    //Creamos el vector que leera la imagen comprimida, del tamaño que nos dio el paso anterior
    std::vector<unsigned int> lectura;
    lectura.resize(tamano/(sizeof(unsigned int)));
    
    //Lee el contenido entero del archivo al vector
    f.read((char *) &lectura.front(), lectura.size()*sizeof(lectura.front()));
    //Cerrramos el archivo
    f.close();
   
    //A partir del vector, reconstruimos la imagen
    CImg<bool> img_leida;
    img_leida = getRLCInverse(lectura);
    return img_leida;
}

int main(int argc, char *argv[]) {
    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const unsigned int _umbral = cimg_option("-u", 100.0, "Input Image File");
    const bool inicio = cimg_option("-s", false, "Valor de inicio");

    std::string filename = "ej2-rlc.jpg";
    //Leemos la imagen
    CImg<double> imagen(_input);
    //Comprimimos
    CImg<double> imagen_binaria = RLCCompression(imagen,_umbral, filename, inicio);
  
    //Descomprimimos
    CImg<bool> img_leida = RLCDecompression(filename);
    
    std::cerr<<"Error en la compresion-descompresion = "<<img_leida.MSE(imagen_binaria)<<"\n\n\n";
    
    (imagen_binaria.get_normalize(0,255), img_leida.get_normalize(0,255)).display();

    return 0;
}
