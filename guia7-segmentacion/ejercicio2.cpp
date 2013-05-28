#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include "../utils/PDI_functions.h"

using namespace cimg_library;   //Necesario


CImg<double> hough_preferencia(CImg<double> img, double angulo, double ro, bool inverse=false) {

  CImg<double> iHough(img); iHough.fill(0.0);
  const unsigned M = img.width(),
                 N = img.height();
  
  double max_rho = sqrt(float(pow(N-1,2)+pow(M-1,2))), //maximo valor posible de radio se da en la diagonal pcipal
         step_rho = 2.*max_rho/(N-1), //paso en eje rho (rho=[-max_rho , max_rho])
         step_theta = M_PI/(M-1),     //paso en eje theta (M_PI=pi) (theta=[-90,90])
         rho, theta;
	 
  if (!inverse){ 
    int r;  // radio mapeado en los N pixeles
    cimg_forXY(img,y,x){
      if (img(y,x) > 0.5)
        for (int t=0; t<M; t++) { //calculo rho variando theta en todo el eje, con x e y fijo
          theta=t*step_theta-M_PI/2;  // mapea t en [0,M-1] a t en [-90,90]
          rho=x*cos(theta)+y*sin(theta); // calcula rho para cada theta

          if ( fabs(theta-angulo) < 0.0001 ) {
          	r=floor((rho+max_rho)/step_rho+.5); // mapea r en [-max_rho , max_rho] a r en [0,N-1] el floor(r+.5) sirve para redondear
          	iHough(t,r)+= 1;               // suma el acumulador     
          }
        }
    } 
  }else{
    const double blanco[1] = {255.f};
    float x0, x1, y0, y1;
    cimg_forXY(img,t,r){
      if (img(t,r) > 0.5) {
        theta=t*step_theta-M_PI/2;   // mapea t en [0,M-1] a t en [-90,90]
        rho=r*step_rho-max_rho;      // mapea r en [0,N-1] a r en [-max_rho,max_rho]
        if (theta>-M_PI/2 && theta<M_PI/2){
	  y0=0; y1=M-1;
          x0=rho/cos(theta);      // calcula y para y=0
          x1=rho/cos(theta)-(M-1)*tan(theta); // calcula y para y=M-1	  
	}else{
	  x0=0; x1=N-1;
          y0=rho/sin(theta);      // calcula y para x=0
          y1=rho/sin(theta)-(N-1)/tan(theta); // calcula y para x=M-1
	}
	//cout<<endl<<"("<<t<<","<<r<<")->("<<theta<<","<<rho<<") "<<"("<<y0<<","<<x0<<")->("<<y1<<","<<x1<<")"<<endl;
        iHough.draw_line(y0,x0,y1,x1,blanco); // dibuja una línea de (0,y0) a (M-1,y1)
      }
    }
  }
  return iHough; 
}

CImg<double> generar_test(unsigned int w, unsigned int h, unsigned int cantidad, bool colineales = false) {
	CImg<double> retval(w,h,1,1,0);

	if (colineales) {
			unsigned int x = rand()%w;
			unsigned int y = rand()%h;
			for (unsigned int i = 0; i < cantidad; i++) {
				retval( (x+i*10)%w , (y+i*10)%h ) = 255;
			}

	}else{
		for (unsigned int i = 0; i < cantidad; i++) {
			unsigned int x = rand()%w;
			unsigned int y = rand()%h;

			retval(x,y) = 255;
		}
	}
	return retval;
}


int main(int argc, char *argv[]) {
 	const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
 	const double angulo = cimg_option("-a", 90.0f , "Input Image File");
 	const double ro = cimg_option("-r", 90.0f , "Input Image File");

 	CImg<double> imagen(input);

 	CImg<double> transformada_hough =  hough_preferencia(imagen, angulo, ro);

 	(imagen, transformada_hough).display();
    return 0;
}
