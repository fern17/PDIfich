#include <CImg.h>               //include basico de CImg
#include <cassert>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario

CImg<double> restarImg(CImg<double> & img1, CImg<double> & img2) {
    assert( img1.is_sameXY(img2) );

    CImg<double> salida(img1.width(), img1.height(), 1, 1, 0);

    double temporal;
    cimg_forXY(img1,x,y) {
        temporal = (255 + img1(x,y) - img2(x,y) ) / 2.0;
        if (temporal < 0.0) temporal = 0.0;
        else if (temporal > 255.0) temporal = 255.0;
        
        salida(x,y) = temporal;
    }
    return salida;
}
    
int main(int argc, char *argv[]) {
    //@ Leer filtro acentuado, aplicar filtro acentuado, convolve con filtro de distinta medida. Filtrado de Alta potencia

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");
    const float _scalar = cimg_option("-e", 1.2, "Input filter File");


    CImg<double> img(_input), filtrada(img.width(), img.height(), 1, 1, 0), output, tempy;

    //Leemos el filtro de acentuado
    CImg<double> filtro = utils::get_filtro(_filter);

    //Primer miembro
    tempy = _scalar * img;

    //Segundo miembro
    filtrada = img.get_convolve(filtro);

    //Salida
    output = restarImg(tempy,filtrada);

    CImgList<double> lista;
    lista.assign(img, tempy, output.normalize(0,255) );
    lista.display();
}
