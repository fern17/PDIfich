#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include "../../utils/MYPDI_functions.h"

using namespace cimg_library;   //Necesario
using namespace std;





int main(int argc, char *argv[]) {
	const char* _input = cimg_option("-i", "../img/scanner/escaneo3.png", "Input Image File");
	const double _umbral = cimg_option("-u", 0.1f, "Input Image File");
	CImg<double> imagen(_input);
	if (imagen.spectrum() != 1)
		imagen = imagen.get_RGBtoHSI().get_channel(2);

	int color[] = { 255, 255 , 255 };

	CImg<double> transformada = magn_tdf(imagen, true, "../../paletas/gray.pal").normalize(0,255);
	CImg<bool> mascara = getMascaraMayores(transformada, 150);

	CImg<double> umbral = aplicarMascara(imagen, mascara);
	CImg<double> though = hough(umbral);
	std::vector< maximo<double> > maximos;
	maximos = getNMax(though,2);

	std::vector<unsigned int> v;
	v = coordHoughToImg<double>(imagen, maximos[0].x, maximos[0].y );

	// imagen.draw_line(v[0], v[1], v[2], v[3], color );
	double angle = atan2( (v[2]-v[0]) , (v[3]-v[1]) ) * 180;

	double angle2 = ((double)maximos[0].x * M_PI / (double)(imagen.width() - 1) - M_PI/2) * 180;

	CImg<double> retval(imagen.width(), imagen.height(), 1, 1, 0);
	retval.draw_line(v[0],v[1],v[2],v[3], color);

	std::cout<<maximos[0].x<<" "<<angle2<<std::endl;
	( imagen, transformada ,  retval, imagen.get_rotate( angle ), imagen.get_rotate( -angle ) ) .display();
	return 0;
}
