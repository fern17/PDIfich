namespace utils{
#include <vector>
#include <cmath>
#include <CImg.h>

using namespace cimg_library;   //Necesario
template<typename T>
std::vector<unsigned int> coordHoughToImg(CImg<T> imagen, unsigned int _tita, unsigned int _rho) {
    unsigned int M = imagen.width();
    unsigned int N = imagen.height();

    double step_tita = M_PI/double(M-1.0);
    double diagonal = std::sqrt(std::pow(M-1,2) + std::pow(N-1,2));
    double step_rho = 2.0*diagonal/double(N-1);
    //Actualiza tita y rho a valores reales
    double tita = double(_tita)*step_tita - M_PI/2.0; 
    double rho  = double(_rho)*step_rho - diagonal;

    unsigned int x0;
    unsigned int x1;
    unsigned int y0;
    unsigned int y1;
    
    if (tita>-M_PI/2 && tita<M_PI/2){
        y0 = 0; 
        y1 = M-1;
        x0 = rho/cos(tita);      // calcula y para y=0
        x1 = rho/cos(tita)-(M-1)*tan(tita); // calcula y para y=M-1	  
    }else{
        x0 = 0; 
        x1 = N-1;
        y0 = rho/sin(tita);      // calcula y para x=0
        y1 = rho/sin(tita)-(N-1)/tan(tita); // calcula y para x=M-1
    }
    unsigned int ret_val[4] = {y0,x0,y1,x1};
    std::vector<unsigned int> retval;
    retval.assign(&ret_val[0], &ret_val[0]+4);
    return retval;
}
}
