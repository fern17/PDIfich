#include <CImg.h>               //include basico de CImg
#include <cassert>
#include <vector>
#include "../utils/genArchivoMascara.cpp"

using namespace cimg_library;   //Necesario

CImg<unsigned char> restarImg(CImg<unsigned char> & img1, CImg<unsigned char> & img2) {
    assert( img1.is_sameXY(img2) );
    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    int temporal;
    cimg_forXY(img1,x,y) {
        temporal = (255 + img1(x,y) - img2(x,y) ) / 2;
        if (temporal < 0) temporal = 0;
        else if (temporal > 255) temporal = 255;
        
        salida(x,y) = temporal;
    }
    return salida;
}
    
int main(int argc, char *argv[]) {
    //@ Leer filtro acentuado, aplicar filtro acentuado, convolve con filtro de distinta medida, Filtrado de Mascara Difusa

    const char* _input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* _filter = cimg_option("-m", "filtro_ej3.txt", "Input filter File");

    CImg<unsigned char> img(_input), filtrada(img.width(), img.height(), 1, 1, 0), output;

    //Leemos el filtro de acentuado
    CImg<float> filtro = utils::get_filtro(_filter);

    //Filtramos
    filtrada = img.get_convolve(filtro);

    //Le restamos la otra imagen
    output = restarImg(img,filtrada);

    CImgList<double> lista;
    lista.assign(img, output.normalize(0,255) );
    lista.display();
}
