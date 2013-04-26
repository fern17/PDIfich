#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include "../utils/bresenham.cpp"
#include <vector>

using namespace cimg_library;   //Necesario

//Struct que representa un click de un mouse
typedef struct click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
}click;

unsigned int CANTIDAD_CLICK = 0;
//Primer y segundo click
click click1;
click click2;

//Distancia euclidea entre dos click
float dist(click c1, click c2) {
    float dx = c2.x-c1.x;
    float dy = c2.y-c1.y;
    return sqrt(dx*dx + dy*dy);
}

//@ Retorna el perfil de intensidad a traves de un segmento en cualquier direccion. Hace uso de las variables globales click1 y click2
CImg<unsigned char> obtenerPerfilDiagonal(CImg<unsigned char> img) {
    std::vector<std::vector<unsigned int> > puntos_a_incluir;       //Aqui se guardaran los pares de coordenadas 
    
    utils::get_bresenham_coords(img.height(), img.width(), click1.x, click1.y, click2.x, click2.y, puntos_a_incluir); //obtenemos las coordenadas
    CImg<unsigned char> recortado(puntos_a_incluir.size(), 1);    //Creamos una imagen de una fila
    
    //Recorremos el vector de coordenadas y copiamos los valores de cada pixel de la imagen
    for (unsigned int i = 0; i < puntos_a_incluir.size(); i++) {
        //std::cout<<puntos_a_incluir[i][0]<<' '<<puntos_a_incluir[i][1]<<'\n';
        recortado(i,0) = img(puntos_a_incluir[i][0], puntos_a_incluir[i][1]);
    }
    
    return recortado;
}

//@ Obtiene el perfil de una imagen y lo devuelve en cortada
bool obtenerPerfil(CImg<unsigned char> &img, CImg<unsigned char> &cortada) {
    if (dist(click1,click2) < 4) {
        //estan muy cerca, no sirve
        std::cout<<"Muy cerca, no sirve\n";
        return false;
    }
    
    if (fabs(click2.x-click1.x) < 3) {
        //vertical
        std::cout<<"Vertical\n";
        cortada = img.get_crop(click1.x,click1.y,click1.x,click2.y); 
    
    } else if (fabs (click2.y - click1.y) < 3) {
        //horizontal
        std::cout<<"Horizontal\n";
        cortada = img.get_crop(click1.x,click1.y,click2.x,click1.y);
    } else {
        std::cout<<"Diagonal\n";
        cortada = obtenerPerfilDiagonal(img); 
        /*
        //float pendiente = (click2.y - click1.y)/(click2.x - click1.x);
        float dx = click2.x - click1.x;
        float dy = click2.y - click1.y;
        float angulo = atan2(dy,dx) * 180/M_PI;
        std::cout<<"Angulo a rotar "<<angulo<<'\n';
        CImg<unsigned char> rotada = img.get_rotate(angulo);
        unsigned int  x0r =  click1.x*cos(angulo) + click1.y*sin(angulo),
                      y0r = -click1.x*sin(angulo) + click1.y*cos(angulo),
                      x1r =  click2.x*cos(angulo) + click2.y*sin(angulo),
                      y1r = -click2.x*sin(angulo) + click2.y*cos(angulo);

        cortada = img.get_crop(x0r,y0r,x1r,y1r);
        */
    }

    return true;
}


//se supone que img es un arreglo de los valores de la imagen recortados
CImg<unsigned char> perfilIntesidad(CImg<unsigned char> img) {
    CImg<unsigned char> recortado; 
    if(obtenerPerfil(img,recortado)) 
        return recortado;
    else
        return img;
    
}

//@ Registra los dos clicks y al tenerlos, llama a perfilIntesidad
template<typename T>
void loopDisplayAndClick(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    CImg<unsigned char> visu(500,400,1,3,0);
    CImgDisplay ventana2 (visu, "perfil");   //Crea una imagen donde se va a dibujar el perfil.
    const unsigned char red[] = { 255,0,0 };
    while ( not ventana.is_closed() && not ventana.is_keyQ()) {
        ventana.wait(); //espera eventos
        if (ventana.button() && ventana.mouse_y() >= 0 && ventana.mouse_x() >= 0) { 
            //si se apreta un boton del mouse dentro de la ventana
            if (CANTIDAD_CLICK % 2 == 0){ //primer click
                click1.x = ventana.mouse_x();
                click1.y = ventana.mouse_y();
                std::cout<<"Click1 en "; click1.print(true); 
            } else {    //segundo click
                click2.x = ventana.mouse_x();
                click2.y = ventana.mouse_y();
                std::cout<<"Click2 en "; click2.print(true);
                //Como ya tenemos dos clicks, estamos listos para calcular el perfil
                CImg<unsigned char> perfil = perfilIntesidad(img);
                //Dibuja el perfil
                visu.fill(0).draw_graph(perfil, red, 1,1,1,0,255).display(ventana2); 
            }
            CANTIDAD_CLICK++;
            if (CANTIDAD_CLICK == 2) //acotamos para evitar overflow
                CANTIDAD_CLICK = 0;
        }
    }
}



int main(int argc, char *argv[]) {
    //Imprime información básica de la librería
    cimg_usage("Obtencion de perfiles de intensidad en cualquier direccion");
    
    //Especifica parámetros para el ejecutable.
    // -i: opcion
    // "cameraman.tif": valor por defecto
    // "Input..": descripcion de la opcion, para usarla con -h
    //
    // Para usar como: ejercicio1.bin -i "lena.jpg" -o "salida.jpg"
    const char* input  = cimg_option("-i", "../images/cameraman.tif", "Input Image File");
    const char* output = cimg_option("-o", "output1.bmp", "Output Image File");

    CImg<unsigned char> img(input);
    loopDisplayAndClick(img);

    return 0;
}
