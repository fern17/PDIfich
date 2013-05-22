
#include <CImg.h>
#include <cmath>

using namespace cimg_library;   //Necesario

CImg<double> transformadaLogaritmo(CImg<double> img, double cc) {
    CImg<double> ret_val(img.width(), img.height(), img.depth(), img.spectrum(), 0.0);
    CImg<double> img_n = img.get_normalize(0.0,1.0);
    cimg_forC(img_n, c) {
        cimg_forXY(img_n,x,y) {
            double val = cc * std::log(1 + img_n(x,y,0,c));
            ret_val(x,y,0,c) = val;
        }
    }
    return ret_val;
}
