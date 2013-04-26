#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cassert>
using namespace cimg_library;   //Necesario

//@ Devuelve el plano de bits pedido
CImg<bool> getBitPlane(CImg<unsigned char> & img1, int bit){
	CImg<bool> img(img1);
	img.fill(0);
	img = img1 & ( pow(2,bit) );
	return img;
}

//@ Calcula el error cuadratico medio entre dos imagenes
float getECM(CImg<unsigned char> &img1, CImg<unsigned char> &img2) {
    assert(img1.is_sameXY(img2));

    float salida = 0;
    //Formula del error
	cimg_forXY(img1, x,y) {
        salida += pow(img1(x,y) - img2(x,y),2);
    }
    //Normalizacion por los tamaños
    salida = salida / ( img1.width() * img1.height() );

    return salida;
}


int main(int argc, char *argv[]) {
    //@ Utilizacion de las rodajas de bits y calculo de error
    cimg_usage("Utilizacion de las rodajas de bits");
    
    const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    
    CImg<unsigned char> imagen(input);

    CImgDisplay ventana1, ventana2;


    //Lista donde se van a guardar todas las rodajas de bits
    CImgList<unsigned char> lista;

    //Calculamos todos los planos y los agregamos en lista. Ademas imprimimos el error de cada plano con la imagen original
    for (unsigned int i = 0; i < 8; i++) {
    	CImg<unsigned char> plano;
    	
        plano = getBitPlane(imagen, i) * pow(2,i);
    	lista.push_back(plano);

    	std::cout<<"Bit plane #"<<i<<"\t ECM: "<<getECM(imagen, plano)<<"\t MSE: "<<imagen.MSE(plano)<<std::endl;
    }
    lista.display(ventana1, false);

    CImg<unsigned char> temporal = lista.at(7);

    //Sumamos los planos y vamos viendo el error acumulado
    CImgList<unsigned char> acumulado;
    acumulado.push_back(temporal);
    
    for (int i = 6; i >= 0; i--) {
        temporal = temporal + lista.at(i);
    	acumulado.push_back(temporal);
    	
        std::cout<<"Acumulado bits #7-"<<i<<"\t ECM: "<<getECM(imagen, temporal)<<"\t MSE: "<<imagen.MSE(temporal)<<std::endl;
    }

    acumulado.display(ventana2, false);
    return 0;
}
