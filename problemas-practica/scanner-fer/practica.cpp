#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "MyCImg.cpp"
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {
 	const unsigned char par1 = cimg_option("-j", 6, "p1");
    const double par2 = cimg_option("-k", 1.0, "p2");
    const double umbral_fourier = cimg_option("-u", 128.0, "p2");

    std::vector<std::string> archivos;

 	archivos.push_back("img/01.png");
 	archivos.push_back("img/02.png");
 	archivos.push_back("img/03.png");
 	archivos.push_back("img/04.png");

 	for (int i = 0; i < archivos.size(); i++){
        CImg<double> img(archivos[i].c_str());
        CImg<double> img_original = img;
        unsigned int h = img.height();
        unsigned int w = img.width();
        //img.display();
        img.convolve(operadorSobel()[0]);
        img.convolve(operadorSobel()[1]);
        img.blur(10);
        img = getMayorAUmbral(img.normalize(0,255), 10.0);
        //img.display();
        
        CImg<double> tf = magn_tdf(img,true,"../../images/paletas/gray.pal");
        CImg<double> tfu = getMayorAUmbral(tf, umbral_fourier);
        (tf, tfu).display();

        CImg<double> th = hough(img);
        double maxi = th.max();
        unsigned int tita;
        unsigned int rho;
        cimg_forXY(th,x,y){
            if (fabs(th(x,y) - maxi) < EPS) {
                tita = x; rho = y; 
            }
        }
        std::cerr<<"Maximo en "<<tita<<' '<<rho<<'\n';
        std::vector<unsigned int> coords = coordHoughToImg(img, tita, rho);
        img.draw_line(coords[0], coords[1], coords[2], coords[3], blanco);
        double angle = atan2(coords[2] - coords[1], coords[3] - coords[1]);
        img_original.rotate(angle);
        (img, img_original).display();
 	}

    return 0;
}
