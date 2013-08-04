#include <CImg.h>
#include <cmath>

using namespace cimg_library;   //Necesario

CImg<double> correccionGamma(CImg<double> img, double gamma, double A) {
    CImg<double> ret_val(img.width(), img.height(), img.depth(), img.spectrum(), 0.0);
    cimg_forC(img, c) {
        cimg_forXY(img,x,y) {
            double val = A*pow(std::fabs(img(x,y,0,c)),gamma); //agregado fabs
            ret_val(x,y,0,c) = val;
        }
    }
    return ret_val;
}


CImg<double> correccionGammaIntensidad(CImg<double> img, double gamma, double A) {
    CImg<double> ret_val(img.width(), img.height(), img.depth(), 0, 0.0);
    
    CImg<double> img_hsi = img.get_RGBtoHSI();
    CImg<double> intensidad = img_hsi.get_channel(2);

    cimg_forXY(intensidad,x,y) {
        intensidad(x,y) = A*pow(intensidad(x,y),gamma);
    }
    ret_val.append(img_hsi.get_channel(0) , 'c');
    ret_val.append(img_hsi.get_channel(1) , 'c');
    ret_val.append(intensidad , 'c');
    ret_val.HSItoRGB();
    return ret_val;
}
