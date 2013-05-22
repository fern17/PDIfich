#include <CImg.h>
#include <cmath>

using namespace cimg_library;   //Necesario

CImg<double> correccionGamma(CImg<double> img, double gamma) {
    CImg<double> ret_val(img.width(), img.height(), img.depth(), img.spectrum(), 0.0);
    cimg_forC(img, c) {
        cimg_forXY(img,x,y) {
            double val = pow(img(x,y,0,c),gamma);
            ret_val(x,y,0,c) = val;
        }
    }
    return ret_val;
}
