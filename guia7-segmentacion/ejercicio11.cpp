#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
using namespace cimg_library;   //Necesario


//@ Devuelve el operador de Roberts para el gradiente en X [0] e Y [1]
CImgList<double> operadorRoberts(){
    CImgList<double> ret_val(2,2,2,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,1) = 1;

    //Gradiente en Y
    ret_val[1](0,1) = 1;
    ret_val[1](1,0) = -1;
    return ret_val;
}

//@ Devuelve el operador de Prewitt para el gradiente en las 4 direcciones
CImgList<double> operadorPrewitt(){
    CImgList<double> ret_val(4,3,3,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,0) = -1;
    ret_val[0](2,0) = -1;
    ret_val[0](0,2) = 1;
    ret_val[0](1,2) = 1;
    ret_val[0](2,2) = 1;

    ret_val[1](0,0) = -1;
    ret_val[1](0,1) = -1;
    ret_val[1](0,2) = -1;
    ret_val[1](2,0) = 1;
    ret_val[1](2,1) = 1;
    ret_val[1](2,2) = 1;

    ret_val[2](0,1) = -1;
    ret_val[2](0,2) = -1;
    ret_val[2](1,2) = -1;
    ret_val[2](1,0) = 1;
    ret_val[2](2,0) = 1;
    ret_val[2](2,1) = 1;

    ret_val[3](0,0) = -1;
    ret_val[3](0,1) = -1;
    ret_val[3](1,0) = -1;
    ret_val[3](2,2) = 1;
    ret_val[3](1,2) = 1;
    ret_val[3](2,1) = 1;

    return ret_val;
}

//@ Devuelve el operador de Sobel para el gradiente en las 4 direcciones
CImgList<double> operadorSobel(){
    CImgList<double> ret_val(4,3,3,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,0) = -2;
    ret_val[0](2,0) = -1;
    ret_val[0](0,2) = 1;
    ret_val[0](1,2) = 2;
    ret_val[0](2,2) = 1;

    ret_val[1](0,0) = -1;
    ret_val[1](0,1) = -2;
    ret_val[1](0,2) = -1;
    ret_val[1](2,0) = 1;
    ret_val[1](2,1) = 2;
    ret_val[1](2,2) = 1;

    ret_val[2](0,1) = -1;
    ret_val[2](0,2) = -2;
    ret_val[2](1,2) = -1;
    ret_val[2](1,0) = 1;
    ret_val[2](2,0) = 2;
    ret_val[2](2,1) = 1;

    ret_val[3](0,1) = -1;
    ret_val[3](0,0) = -2;
    ret_val[3](1,0) = -1;
    ret_val[3](1,2) = 1;
    ret_val[3](2,2) = 2;
    ret_val[3](2,1) = 1;


    return ret_val;
}

//@ Devuelve una mascara llena de -1 excepto en el centro donde tiene un 8
CImgList<double> operadorLaplaciano8() {
    CImgList<double> ret_val(1,3,3,1,1,-1);
    ret_val[0](1,1) = 8; 
    return ret_val; 
}

//@ Devuelve una mascara de 3x3 con un 4 en el medio y cuatro -1 en los extremos
CImgList<double> operadorLaplaciano4() {
    CImgList<double> ret_val(1,3,3,1,1,0);
    ret_val[0](1,1) = 4; 

    ret_val[0](0,1) = -1; 
    ret_val[0](1,0) = -1; 
    ret_val[0](1,2) = -1; 
    ret_val[0](2,1) = -1; 
    
    return ret_val; 
}

//@ Devuelve la mascara de 5x5 para el LoG
CImgList<double> operadorLoG() {
    CImgList<double> ret_val(1,5,5,1,1,0);
    ret_val[0](2,0) = -1; 
    ret_val[0](1,1) = -1; 
    ret_val[0](3,1) = -1; 
    ret_val[0](4,2) = -1; 
    ret_val[0](3,3) = -1; 
    ret_val[0](2,4) = -1; 
    ret_val[0](1,3) = -1; 
    ret_val[0](0,2) = -1; 
    
    ret_val[0](1,2) = -2; 
    ret_val[0](2,1) = -2; 
    ret_val[0](3,2) = -2; 
    ret_val[0](2,3) = -2;
    
    ret_val[0](2,2) = 16;

    return ret_val;
}


//@ Aplica el operador derivada segun el parametro opcion
//0: Gradiente de Roberts
//1: Gradiente de Prewitt
//2: Gradiente de Sobel
//3: Laplaciano de 4 vecinos
//4: Laplaciano de 8 vecinos
//5: LoG, Laplaciano del Gaussiano
//Devuelve una lista con todos los resultados de aplicar todas las mascaras del operador en particular
CImgList<double> aplicarDerivada(CImg<double> img, unsigned int opcion = 0) {
    CImgList<double> derivada;
    if (opcion == 0) 
        derivada = operadorRoberts();
    if (opcion == 1) 
        derivada = operadorPrewitt();
    if (opcion == 2) 
        derivada = operadorSobel();
    if (opcion == 3)
        derivada = operadorLaplaciano4();
    if (opcion == 4)
        derivada = operadorLaplaciano8();
    if (opcion == 5)
        derivada = operadorLoG();

    CImgList<double> resultados;
    unsigned int cantidad = derivada.size();
    for (unsigned int i = 0; i < cantidad; i++) {
        resultados.push_back(img.get_convolve(derivada[i]));
    }
     
    return resultados;
}

//@ Aplica ruido y luego aplica los operadores derivada
void mosquito(const char * input, const double umbral, unsigned int opcion, double desvio) {
    CImg<double> img(input);
    CImg<double> img_original = img;

    //Convertimos a HSI y nos quedamos con el canal de intesidad
    img.RGBtoHSI();
    img = img.get_channel(2);

    CImg<double> resultado;
    //Aplicamos el operador y devolvemos las imagenes aplicadas
    CImgList<double> resultados = aplicarDerivada(img, opcion);
    //Normalizamos a 0-255
    for (int i = 0; i < resultados.size(); i++) 
        resultados[i].normalize(0.0,255.0);

    resultado = 5*img_original;
    for (int i = 0; i < resultados.size(); i++) {
        resultado = resultado + 0.5*resultados[i];
    }

    //Dibujamos
    (img_original, resultado).display();
}


int main(int argc, char *argv[]) {
    //@ Lee una imagen y le aplica los distintos operadores de derivada
 	const char* input2 = cimg_option("-j", "../images/mosquito.jpg", "Input Image File");
	const double umbral = cimg_option("-u", 0.2, "Umbral para la deteccion de bordes");
	const unsigned int opcion = cimg_option("-o", 3, "Operador a aplicar");
	const double desvio = cimg_option("-d", 10, "Desvio del ruido gaussiano");
   
    mosquito(input2, umbral, 2, desvio);
    return 0;
}
