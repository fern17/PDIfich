#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
using namespace cimg_library;   //Necesario


//Funcion que toma una imagen cualquiera, y un titulo de ventana, y la muestra
template<typename T>
void disp(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        //bucle infinito
    }
}



int main(int argc, char *argv[]) {



    
bool patron_grises[10][3][3] = {
                        {{1,1,1},{1,1,1},{1,1,1}},
                        {{1,0,1},{1,1,1},{1,1,1}},
                        {{1,0,1},{1,1,1},{1,1,0}},
                        {{0,0,1},{1,1,1},{1,1,0}},
                        {{0,0,1},{1,1,1},{0,1,0}},
                        {{0,0,0},{1,1,1},{0,1,0}},
                        {{0,0,0},{1,1,0},{0,1,0}},
                        {{0,0,0},{1,1,0},{0,0,0}},
                        {{0,0,0},{0,1,0},{0,0,0}},
                        {{0,0,0},{0,0,0},{0,0,0}}
                     };







    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");

    const char* input = cimg_option("-o", "../images/huang2.jpg", "Output Image File");

    CImg<unsigned char> img_original(input);



    CImg<unsigned char> img(256,256);

    cimg_forXY(img,x,y) {
        img(x,y) = x;
    }
    img_original = img;




    CImg<unsigned char> muy_original = img_original;


    // img_original.quantize(10); img_original.normalize(0,9);
    unsigned int h = img_original.height();
    unsigned int w = img_original.width();

    CImg<bool> img_resultado(w*3, h*3);

    float factor = 255/9;

    cimg_forXY(img_original,x,y) {

        unsigned int current_level = 10 - img_original(x,y) /factor;
        // unsigned int current_level = 9 - img_original(x,y) ;

        // std::cout<<current_level<<'\n';
        // std::cout<<img_original(x,y)<<'\n';
        // std::getchar();

        //Estos son mapeos a cada submatriz de 3x3

        img_resultado(3*x, 3*y) = patron_grises[current_level][0][0];
        img_resultado(3*x+1, 3*y) = patron_grises[current_level][0][1];
        img_resultado(3*x+2, 3*y) = patron_grises[current_level][0][2];

        img_resultado(3*x, 3*y+1) = patron_grises[current_level][1][0];
        img_resultado(3*x+1, 3*y+1) = patron_grises[current_level][1][1];
        img_resultado(3*x+2, 3*y+1) = patron_grises[current_level][1][2];

        img_resultado(3*x, 3*y+2) = patron_grises[current_level][2][0];
        img_resultado(3*x+1, 3*y+2) = patron_grises[current_level][2][1];
        img_resultado(3*x+2, 3*y+2) = patron_grises[current_level][2][2];
        
    }
    
disp(img_resultado);
disp(img_resultado.resize(w,h));
disp(muy_original);

    // std::vector<CImg <unsigned char> > img_vector;

    // CImgList<unsigned char> lista;
    // lista.push_back(img_original);
    // lista.push_back(img_resultado.resize(w,h));


    // CImgDisplay ventana(lista);
    //   while ( not ventana.is_closed() && not ventana.is_keyQ()) {
    //     //bucle infinito
    // }

    return 0;
}
