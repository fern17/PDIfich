#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
using namespace cimg_library;   //Necesario

bool colorSimilar(CImg<unsigned char> img, unsigned char color_comparacion[], unsigned char umbral) {
    return ((abs(img(0,0,0,0) - color_comparacion[0]) + 
            abs(img(0,0,0,1) - color_comparacion[1]) + 
            abs(img(0,0,0,2) - color_comparacion[2])) < umbral);
}

CImg<unsigned char> getZonaInteres(CImg<unsigned char> img, unsigned char color_fondo[], unsigned char umbral) {
    unsigned int min_x = 0;
    unsigned int max_x = img.width();
    unsigned int min_y = 0;
    unsigned int max_y = img.height();

    cimg_forXY(img, x, y) {
        if (not colorSimilar(img.get_crop(x,y,x,y), color_fondo, umbral)) {
            if (x > min_x) min_x = x;
            if (x < max_x) max_x = x;
            if (y > min_y) min_y = y;
            if (y < max_y) max_y = y;
        }
    }
    return img.get_crop(min_x, min_y, max_x, max_y);
}

int main(int argc, char *argv[]) {
    //@ 
 	const char* _input = cimg_option("-i", "../img/cervezas/01.jpg", "Input Image File");

 	const unsigned char umbral = cimg_option("-u", 6, "Umbral de permiso para getzonainteres");
 	const unsigned char umbral_negra = cimg_option("-n", 100, "Umbral de comparacion entre negra y rubia");
 	const unsigned char area = cimg_option("-n", 10, "Area ");
    unsigned char rojo[] = {255, 0, 0};
    unsigned char verde[] = {0,255,0};
    unsigned char azul[] = {0,0,255};
    unsigned char blanco[] = {255,255,255};

 	std::vector<std::string> archivos;

 	archivos.push_back("../img/cervezas/01.jpg");
 	archivos.push_back("../img/cervezas/05.jpg");
 	archivos.push_back("../img/cervezas/08.jpg");
 	archivos.push_back("../img/cervezas/11.jpg");
 	archivos.push_back("../img/cervezas/12.jpg");
 	archivos.push_back("../img/cervezas/13.jpg");
 	archivos.push_back("../img/cervezas/31.jpg");
 	archivos.push_back("../img/cervezas/32.jpg");
 	archivos.push_back("../img/cervezas/33.jpg");
 	archivos.push_back("../img/cervezas/34.jpg");
    unsigned char color_fondo[] = {255,255,255};

 	for (int i = 0; i < archivos.size(); ++i)	{
        CImg<unsigned char> imagen(archivos[i].c_str());
        CImg<unsigned char> zona_interes = getZonaInteres(imagen, color_fondo, umbral);
        int analisis_x = zona_interes.width() / 2;
        int analisis_y = zona_interes.height() / 2;
        CImg<unsigned char> img_interes = zona_interes.get_channel(0);
        double prom_interes = img_interes.get_crop(analisis_x-area/2, analisis_y-area/2, analisis_x+area/2, analisis_y+area/2).mean();
         
        std::cout<<"Prom interes = "<<prom_interes<<'\n';
        if (prom_interes > umbral_negra)
            std::cout<<"La cerveza es rubia\n";
        else
            std::cout<<"La cerveza es negra\n";
        
        //Ahora el interes esta en el canal azul, que presenta una diferencia grande entre fondo y cerveza
        img_interes = zona_interes.get_channel(2);

        //Comparamos prom_prev con prom_actual, moviendonos para arriba
        double prom_prev = img_interes.get_crop(analisis_x-area/2, analisis_y-area/2, analisis_x+area/2, analisis_y+area/2).mean();
        //Buscamos hasta que cambie
        while(true) {
            analisis_y -= 5;
        
            double prom_actual = img_interes.get_crop(analisis_x-area/2, analisis_y-area/2, analisis_x+area/2, analisis_y+area/2).mean();
            if (fabs(prom_actual - prom_prev) > 10 or analisis_y < 0)
                break;
            else
                prom_prev = prom_actual;
        }
        
        //Aca obtuvimos el inicio de la espuma
        int inicio_espuma = analisis_y;
        std::cout<<"Inicio espuma = "<<inicio_espuma<<'\n';

        //Buscamos para arriba de la espuma hasta llegar al blanco o salir de rango
        while(analisis_y > 0) {
            analisis_y -= 5;
            if ( (colorSimilar(zona_interes.get_crop(analisis_x, analisis_y, analisis_x, analisis_y), blanco, 4)) and
                 (fabs(img_interes.get_crop(analisis_x-area/2, analisis_y-area/2, analisis_x+area/2, analisis_y+area/2).mean()-255.0) < 4.0)){
                break;
            }
        
        }
        //Aca obtuvimos el fin de la espuma
        int fin_espuma = analisis_y;
        std::cout<<"Fin espuma = "<<fin_espuma<<'\n';

        //Buscamos para abajo ahora, reinicializamos las coordenadas
        analisis_x = zona_interes.width() / 2;
        analisis_y = zona_interes.height() / 2;

        //Buscamos para abajo hasta salir de rango o encontrar el blanco
        while(analisis_y < img_interes.height()) {
            analisis_y += 5;
            if ( (colorSimilar(zona_interes.get_crop(analisis_x, analisis_y, analisis_x, analisis_y), blanco, 4)) and
                 (fabs(img_interes.get_crop(analisis_x-area/2, analisis_y-area/2, analisis_x+area/2, analisis_y+area/2).mean()-255.0) < 4.0)){
                break;
            }
        }
        int inicio_cerveza = analisis_y;
        std::cout<<"Inicio cerveza = "<<inicio_cerveza<<'\n';

        std::cerr<<"Porcentaje de Cerveza = "<<double(inicio_cerveza - inicio_espuma)/double(inicio_cerveza - fin_espuma)<<'\n';
        zona_interes.draw_circle(analisis_x, inicio_espuma, 5, rojo);
        zona_interes.draw_circle(analisis_x, fin_espuma, 5, verde);
        zona_interes.draw_circle(analisis_x, inicio_cerveza, 5, azul);

        (imagen, zona_interes
         ).display();
 	}

    return 0;
}
