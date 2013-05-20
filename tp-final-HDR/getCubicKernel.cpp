#include <iostream>
#include <cmath>
#include <vector>

#include <CImg.h>               //include basico de CImg
using namespace cimg_library;   //Necesario

//Retorna un kernel cubico en [a,b] de n elementos
CImg<double> getCubicKernel(unsigned int n, double a = -2.0, double b = 2.0){
    
    std::cout<<"Kernel generado en ["<<a<<','<<b<<"] con "<<n<<" pasos\n";

    //Calculo del paso
    double step = (b-a)/(n+1);
    //Inicializamos el vector
    std::vector<double> x;
    double actual = a;

    for (unsigned int i = 0; i < n; i++) {
        x.push_back(actual);
        actual += step;
    }
    
    //W(x)
    std::vector<double> y;
    CImg<double> ret_val(x.size(), 1, 1, 1);
    for (unsigned int i = 0; i < x.size(); i++) {
        double valor = x[i];
        double xx = fabs(valor);
        double agregar;
        if (xx < 1.0)
            agregar = 1.5*std::pow(xx,3) - 2.5*std::pow(xx,2) + 1.0;
        else if (xx < 2.0)
            agregar = -0.5*std::pow(xx,3) + 2.5*std::pow(xx,2) - 4.0*xx + 2.0;
        else
            agregar = 0.0;
        ret_val(i) = agregar;
        y.push_back(agregar);
    }

    return ret_val;
}

