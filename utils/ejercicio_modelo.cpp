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

    std::vector<std::string> archivos;

 	archivos.push_back("img/01.jpg");
 	archivos.push_back("img/02.jpg");

    std::cout<<intToStr(10)<<'\n';
    std::cout<<strToInt("10");
    return 0;
 	for (int i = 0; i < archivos.size(); i++){
        CImg<unsigned char> imagen(archivos[i].c_str());
        unsigned int h = imagen.height();
        unsigned int w = imagen.width();
        (imagen).display();
 	}

    return 0;
}
