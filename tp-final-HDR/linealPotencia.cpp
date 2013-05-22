#include <CImg.h>
#include <cmath>
using namespace cimg_library;   //Necesario

//@ Aplica una transformada 
CImg<double> linealPotencia(CImg<double> img_origen) {
    CImg<double> img = img_origen.get_normalize(0,1);
    CImg<double> ret_val (img.width(), img.height(), img.depth(), img.spectrum(),0.0);
    cimg_forC(img,c){
        cimg_forXY(img,x,y) {
            double val = img(x,y,0,c);
            double res;
            if (val < 0.25) {
                res = 0.5*sqrt(val);
            } else if (val > 0.75) {
                res = 4.0/3.0*val*val - 4.0/3.0 * val + 1;
            } else {
                res = val;
            }
            ret_val(x,y,0,c) = res;
        }
    }
    return ret_val;
}
