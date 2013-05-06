#include <CImg.h>               //include basico de CImg
#include <iostream>
using namespace cimg_library;   //Necesario

int main(int argc, char *argv[]) {

    //@ Genera una imagen con un sólo gris y le aplica ruido. Dibuja las imagenes y los histogramas
	const char* input = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
	const double sigma_gauss = cimg_option("-g", 10.0, "Desvio del ruido Gaussiano");
	const double sigma_uniforme = cimg_option("-u", 20.0, "Desvio del ruido Uniforme");
	const double sigma_salpimienta = cimg_option("-s", 2.0, "Desvio del ruido Sal y Pimienta");
    
    std::cout<<"Sigma de los ruidos:";
    std::cout<<"\n\tSigma de Gauss = "<<sigma_gauss;
    std::cout<<"\n\tSigma de Uniforme = "<<sigma_uniforme;
    std::cerr<<"\n\tSigma de S&P = "<<sigma_salpimienta<<'\n';
    

    //Carga la imagen
    CImg<double> img(500,500,1,1,0.5);
    
    //Le aplica cada uno de los ruidos
    CImg<double> img_gauss = img.get_noise(sigma_gauss, 0).get_normalize(0,255);
    CImg<double> img_uniforme = img.get_noise(sigma_uniforme, 1).get_normalize(0,255);
    CImg<double> img_salpimienta = img.get_noise(sigma_salpimienta, 2).get_normalize(0,255);

    //Dibuja
    CImgList<double> lista;
    lista.assign(img, img_gauss, img_uniforme, img_salpimienta);
    lista.display();

    //Calcula histogramas de cada imagen
    CImg<unsigned char> h_img = img.get_histogram(256);
    CImg<unsigned char> h_gauss = img_gauss.get_histogram(256);
    CImg<unsigned char> h_uniforme = img_uniforme.get_histogram(256);
    CImg<unsigned char> h_salpimienta = img_salpimienta.get_histogram(256);


    CImgDisplay ventana;
    //Dibujamos los histogramas
    h_img.display_graph(ventana,3);
    h_gauss.display_graph(ventana,3);
    h_uniforme.display_graph(ventana,3);
    h_salpimienta.display_graph(ventana,3);

    return 0;
}
