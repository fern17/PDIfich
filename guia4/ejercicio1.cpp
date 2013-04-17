#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario

CImg<double> inverso_i(CImg<double> &i) {
    CImg<double> salida = i;
    cimg_forXY(i, x, y) {
        salida(x,y) =  1 - i(x,y);
    }
    return salida;

}

CImg<double> inverso_h(CImg<double> &h) {
    CImg<double> salida = h;
    cimg_forXY(h, x, y) {
        if (h(x,y) < 180) {
            salida(x,y) = h(x,y) + 180;
        } else {
            salida(x,y) = h(x,y) - 180;
        }
    }
    return salida;
}


int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Utilizacion de la libreria CImg");
    
    const char* input = cimg_option("-i", "../images/patron.tif", "Input Image File");
    
    CImg<double> img(input), generada, img_hsi, generada_rgb, r, g, b, h, s, i, rr, gg, bb;

    	
    r = img.get_channel(0);
    g = img.get_channel(1);
    b = img.get_channel(2);
    img_hsi = img.get_RGBtoHSI();

    h = img_hsi.get_channel(0);
    s = img_hsi.get_channel(1);
    i = img_hsi.get_channel(2);

    CImgList<double> lista;
    lista.assign(img,img_hsi, r,g,b, h, s ,i );
    //lista.display();


    //ejercicio b
	h.mirror('x');
    generada = img_hsi;	
	cimg_forXY(generada, x  , y) {
        generada(x,y,0,0) = h(x,y); //H va de 0 a 360 segun @escudero89
		generada(x,y,0,1) = 1;      //S va de 0 a 1
		generada(x,y,0,2) = 1;      //I va de 0 a 1
	}

    h = generada.get_channel(0);
    s = generada.get_channel(1);
    i = generada.get_channel(2);

    generada_rgb = generada.get_HSItoRGB();
    rr = generada_rgb.get_channel(0);
    gg = generada_rgb.get_channel(1);
    bb = generada_rgb.get_channel(2);

    CImgList<double> listab;
    listab.assign(generada_rgb, rr, gg, bb, h , s , i );

    listab.display();

    

    //Parte b! (pobre Unión)

    CImg<double> imagen("../images/patron.tif"), copia;

    copia = imagen;
    imagen.RGBtoHSI();

    h = imagen.get_channel(0);
    s = imagen.get_channel(1);
    i = imagen.get_channel(2);

    h = inverso_h(h);
    i = inverso_i(i);
    h.append(s,'c');
    h.append(i,'c');

    imagen =  h;
    
    CImgList<double> listac;
    listac.assign(copia, imagen.HSItoRGB());

    listac.display();


    return 0;
}
