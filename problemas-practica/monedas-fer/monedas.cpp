#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
using namespace cimg_library;   //Necesario

///****************************************
/// Etiquetado de componentes conectadas
///****************************************
CImg<int> label_cc(CImg<int> img, int blanco=1, int nueva_etiqueta=2){
 
    std::vector<int> equiv(nueva_etiqueta+1,0); //vector de equivalencias
    std::vector<int> vecinos;                                     //vector de etiquetas vecinos superiores e izquierda
    int pos, etiqueta, aux;

    cimg_forXY(img,x,y){                     // recorro la imagen
        if (img(x,y)==blanco){             // si es blanco
            vecinos.clear();                     // inicializo 
            if (x && y)                                // si x no es borde izq e y no es borde superior miro el vecino sup izq
                if (img(x-1,y-1)!=0)    vecinos.push_back(img(x-1,y-1)); // si tiene etiqueta la guardo
            if (y)                                         // si y no es borde superior miro vecino superior
                if (img(x,y-1)!=0)        vecinos.push_back(img(x,y-1));     // si tiene etiqueta la guardo
            if (y && x!=img.width()-1)    // si x no es borde derecho e y borde superior miro vecino sup der
                if (img(x+1,y-1)!=0)    vecinos.push_back(img(x+1,y-1)); // si tiene etiqueta la guardo
            if (x)                                         // si x no es borde izquierdo miro vecino izq
                if (img(x-1,y)!=0)        vecinos.push_back(img(x-1,y));     // si tiene etiqueta la guardo
            if (vecinos.empty()) {         // si no tengo vecinos etiquetados debo generar nueva etiqueta
                vecinos.push_back(nueva_etiqueta); // de vecinos voy a sacar la etiqueta que corresponde al pixel
                equiv[nueva_etiqueta]=nueva_etiqueta; // guardo en la posicion nva etiqueta el valor nva etiqueta
                nueva_etiqueta++;                // genero una nueva etiqueta para cdo necesite
                equiv.push_back(0);            // agrego una posicion en equivalencias con 0
            } else {
                for (int i=0; i<vecinos.size()-1; i++)    // controlo la lista de etiquetas vecinas
                    if (vecinos[i]!=vecinos[i+1]){ // si hay diferentes etiquetas en el vecindario anoto
                        etiqueta=vecinos[i];
                        pos=vecinos[i+1];
                        if (pos<etiqueta){ // en la pos de la mayor etiqueta anoto el valor de la menor 
                            aux=etiqueta;
                            etiqueta=pos;
                            pos=aux;
                        }
                        if (equiv[pos]!=etiqueta){ // si tengo una entrada en esa pos reviso la cadena
                            if (equiv[pos]!=pos){
                                aux=etiqueta;
                                etiqueta=equiv[pos];         
                                pos=aux; 
                                while (equiv[pos]!=pos)
                                    pos=equiv[pos];
                             if (equiv[pos]<etiqueta)
                                    etiqueta=equiv[pos];
                            }
                            equiv[pos]=etiqueta;                    
                        }
                    }
            }
            img(x,y)=vecinos.front(); // asigno etiqueta
        }
    } 
    //img.display("Primera Pasada");

/*
    // Muestro como quedo la tabla
    std::cout<<"Tabla de equivalencias"<<"\n\n";
    for (int j=0; j<equiv.size(); j++)
        std::cout<<j<<" "<<equiv[j]<<std::endl;
    std::cout<<std::endl;
*/
    // reasigno la etiqueta
    cimg_forXY(img,x,y)
        if (img(x,y)!=0)
            if (equiv[img(x,y)]!=img(x,y)){
                etiqueta=equiv[img(x,y)];
                while (equiv[etiqueta]!=etiqueta)
                    etiqueta=equiv[etiqueta];
                img(x,y)=etiqueta;
            }
    
    return img;
}

typedef struct Moneda{
    int cx;
    int cy;
    int radio;
    void print() {
        std::cout<<"("<<cx<<","<<cy<<"), r = "<<radio<<'\n';
    }
}Moneda;

bool colorSimilar(CImg<unsigned char> img, unsigned char color_comparacion[], unsigned char umbral) {
    return ((abs(img(0,0,0,0) - color_comparacion[0]) + 
            abs(img(0,0,0,1) - color_comparacion[1]) + 
            abs(img(0,0,0,2) - color_comparacion[2])) < umbral);
}

bool colorSimilar(unsigned char valor, unsigned char comparar, unsigned char umbral) {
    return (abs(valor - comparar) < umbral);
}


CImg<bool> getMascaraRebanado(CImg<unsigned char> img, unsigned char color[], unsigned char umbral) {
    CImg<bool> ret_val(img.width(), img.height(), 1, 1, true);
    cimg_forXY(img, x, y) {
        if (colorSimilar(img.get_crop(x,y,x,y), color, umbral)) {
            ret_val(x,y) = false;
        }
    }
    return ret_val;
}

Moneda calcularCentro(CImg<int> img, int x, int y) {
    int cx = x;
    int cy = y;
    int value = img(x,y);
    while (img(x,y) == value) {
        x++;
    }
    cx = (cx + x)/2;
    while (y < img.height() and img(cx,y) == value) {
        y++;
    }
    int y_max = y;
    y = cy;
    while (y >= 0 and img(cx,y) == value) {
        y--;
    }
    int y_min = y;

    cy = (y_max + y_min) / 2;

    while(x < img.width() and img(x,cy) == value) {
        x++;
    }
    int x_max = x;
    x = cx;
    while (x >= 0 and img(x,cy) == value) {
        x--;
    }
    int x_min = x;
    cx = (x_min + x_max) / 2;
    int radio = abs(x_max - x_min)/2;
    Moneda mon;
    mon.cx = cx;
    mon.cy = cy;
    mon.radio = radio;
    return mon;
}

std::map<int, Moneda>  obtenerCentrosYRadios(CImg<int> img) {
    std::map<int, Moneda> retval;
    cimg_forXY(img, x, y) {
        int val = img(x,y);
        if (retval.find(val) == retval.end()){
            Moneda mon = calcularCentro(img, x, y);
            retval[val] = mon;
        }
    }
    return retval;
}
const unsigned int R_1C = 32; //R
const unsigned int R_2C = 36; //R
const unsigned int R_10C = 38;//A
const unsigned int R_5C = 42; //R
const unsigned int R_20C = 43;//A
const unsigned int R_1E = 45; //P
const unsigned int R_50C = 48;//A
const unsigned int R_2E = 50; //A

bool radioSimilar(int radio, int valor, int umbral) {
    return abs(radio - valor) < umbral;
}

int main(int argc, char *argv[]) {
    //@ 
 	const char* _input = cimg_option("-i", "../img/cervezas/01.jpg", "Input Image File");

 	const unsigned char umbral = cimg_option("-u", 6, "Umbral de permiso para getzonainteres");
 	const unsigned char umbral_radio = cimg_option("-r", 2, "Umbral de comparacion entre radios");
    const unsigned char umbral_color = cimg_option("-c", 20, "Umbral de permiso para color similar");

    unsigned char rojo[] = {255, 0, 0};
    unsigned char verde[] = {0,255,0};
    unsigned char azul[] = {0,0,255};
    unsigned char blanco[] = {255,255,255};

 	std::vector<std::string> archivos;

 	archivos.push_back("../img/monedas/01.jpg");
 	archivos.push_back("../img/monedas/02.jpg");

    unsigned int x_min;
    unsigned int y_min;
    unsigned int x_max;
    unsigned int y_max;

 	for (int i = 0; i < archivos.size(); ++i)	{
        CImg<unsigned char> imagen(archivos[i].c_str());
        unsigned int h = imagen.height();
        unsigned int w = imagen.width();

        CImg<bool> img_monedas = getMascaraRebanado(imagen, blanco, umbral);
        CImg<int> componentes = label_cc(img_monedas);
        std::set<unsigned char> unicos;
        cimg_forXY(componentes, x, y) {
            unicos.insert(componentes(x,y));  
        }
        std::cout<<"Hay "<<unicos.size()-1<<" monedas\n";        
        std::map<int, Moneda> monedas;
        monedas = obtenerCentrosYRadios(componentes);

        float valor = 0.0;
        std::map<int,Moneda>::iterator p = monedas.begin();
        while (p != monedas.end()){
            p->second.print();
            float valor_actual;
            CImg<int> vecindario = imagen.get_crop(p->second.cx-4, p->second.cy-4, p->second.cx+4, p->second.cy+4);
            float color_medio_r = vecindario.get_channel(0).mean();
            float color_medio_g = vecindario.get_channel(1).mean();
            float color_medio_b = vecindario.get_channel(2).mean();
            
            if (colorSimilar(color_medio_r, 150, umbral_color+20) and colorSimilar(color_medio_g, 150, umbral_color+20) and colorSimilar(color_medio_b, 110, umbral_color+10)){
                std::cout<<"Es de amarilla\n";
                if (radioSimilar(p->second.radio,38, 2)) { //es de 10 centavos
                    valor_actual = 0.1;
                } else if (radioSimilar(p->second.radio,44, 2)){//es de 20 centavos
                    valor_actual = 0.2;
                } else if (radioSimilar(p->second.radio,48, 2)){//es de 50 centavos
                    valor_actual = 0.5;
                } else { //es de 2 euros
                    valor_actual = 2.0;
                }
            }
            
            else if (colorSimilar(color_medio_r, 125, umbral_color) and colorSimilar(color_medio_g, 75, umbral_color) and colorSimilar(color_medio_b, 65, umbral_color)){
                std::cout<<"Es de roja\n";
                if (radioSimilar(p->second.radio,32, 2)) { //es de un centavo
                    valor_actual = 0.01;
                } else if (radioSimilar(p->second.radio,36, 2)){//es de dos centavos
                    valor_actual = 0.02;
                } else { //es de 5 centavos
                    valor_actual = 0.05;
                }
            }
            else if (colorSimilar(color_medio_r, 125, umbral_color) and colorSimilar(color_medio_g, 120, umbral_color) and colorSimilar(color_medio_b, 125, umbral_color)){
                std::cout<<"Es de plateada\n";//un euro
                valor_actual = 1.0;
            }
            std::stringstream ss;
            std::string stri;
            ss << valor_actual;
            ss>>stri;

            imagen.draw_text(p->second.cx, p->second.cy, stri.c_str(), azul, blanco);
            valor = valor + valor_actual;
            p++;
        }

        std::cout<<"Hay "<<valor<<" euros\n";
        

        (imagen
         ).display();
 	}

    return 0;
}
