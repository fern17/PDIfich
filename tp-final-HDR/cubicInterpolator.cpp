#include <iostream>
#include <CImg.h>               //include basico de CImg
#include <vector>
using namespace cimg_library;   //Necesario

// Toma un vecindario de 4x4 e interpola el valor dado por dx,dy
// Las formulas se sacaron del libro "Remote Sensing Digital Image Analysis: An Introduction. 4th Edition."
// John A. Richards · Xiuping Jia. Pagina 50
CImg<double> cubicInterpolator(CImg<double> img, double dx, double dy) {
    //La lleva a 4x4
    img.get_resize(4,4);

    //Valor a retornar, un solo pixel de tantos canales como img
    CImg<double> ret_val(1,1,1,img.spectrum(),0);
    
    //Para cada canal de la imagen
    cimg_forC(img,c) {
        //Para cada fila de la imagen
        std::vector<double> vertical;
        //Obtenemos un valor por fila
        for (unsigned int i = 0; i < 4; i++) {
            double val = dx*(dx*(dx*(img(i,3,0,c) - img(i,2,0,c) + img(i,1,0,c) - img(i,0,0,c))+
                        (img(i,2,0,c) - img(i,3,0,c) - 2*img(i,1,0,c) + 2*img(i,0,0,c))) +
                        (img(i,2,0,c) - img(i,0))) +
                        img(i,1,0,c);
            vertical.push_back(val);
        }
        //Ahora obtenemos un unico valor para todo el vecindario
        double final_value = dy*(dy*(dy*(vertical[3] - vertical[2] + vertical[1] - vertical[0]) + 
                                (vertical[2] - vertical[3] - 2*vertical[1] + 2*vertical[0])) +
                                (vertical[2] - vertical[0])) + vertical[1];
        ret_val(0,0,0,c) = final_value;
    }
    return ret_val;
}
