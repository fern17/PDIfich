#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <queue>
#include <complex>
#include <algorithm>


using namespace cimg_library;   //Necesario
using namespace std;

//Epsilon para comparaciones
const double EPS = 0.001;

//Struct que representa un click de un mouse
typedef struct Click{
    int x;
    int y;
    void print(bool cr) {
        std::cout<<x<<' '<<y;
        if (cr)
            std::cout<<'\n';
    }
    
    //Distancia euclidea entre dos click
    float dist(Click c2){
        float dx = c2.x - x;
        float dy = c2.y - y;
        return sqrt(dx*dx + dy*dy);
    }
}Click;

unsigned char rojo[] = {255, 0, 0};
unsigned char verde[] = {0,255,0};
unsigned char azul[] = {0,0,255};
unsigned char blanco[] = {255,255,255};
unsigned char negro[] = {0,0,0};

int strToInt(std::string s) {
    std::stringstream ss;
    ss<<s;
    int t;
    ss>>t;
    return t;
}

std::string intToStr(int t){
    std::stringstream ss;
    ss<<t;
    std::string s = ss.str();
    return s;
}

double radToReg(double rad) {
    return rad*180.0/M_PI;
}

double degToRad(double deg) {
    return deg*M_PI/180.0;
}
//=========================================================================================================================== 
//=========================================================================================================================== 

//Generales
template<typename T>  
void loopDisplayAndClick(CImg<T> img, std::string title);
CImg<bool> drawcircle(unsigned int lado, unsigned int radio);
CImg<bool> getBitPlane(CImg<unsigned char> & img1, int bit);
template<typename T> CImg<T> histogramaLocal(CImg<T> img, unsigned int ancho, unsigned int alto);
CImgList<bool> umbralizarLista(CImgList<double> l_img, double umbral); 
CImg<bool> binarizar(CImg<unsigned int> imagen, unsigned int umbral); 
void BubbleSort(CImg<double> &histograma, std::vector<unsigned char> & grises_ordenados);
CImg<unsigned char> getZonaInteres(CImg<unsigned char> img, unsigned char color_fondo[], unsigned char umbral); 
template<typename T> unsigned int contarValor(CImg<T> imagen, T valor); 
void getPuntosCardinales(CImg<double> img, std::vector<unsigned int> & cardinalidades, double umbral); 
CImg<float> cargar_paleta(const char* filename);
template<typename T> void printVector(std::vector<T> V) ;

//Auxiliares o agregadas por Mari
bool pixel_es_mayor(pixel a, pixel b);
std::vector<pixel> getNMayores(CImg<double>  & though , unsigned int cantidad);
bool es_deseada(CImg<T> v_evaluar, CImg<T> v_referencia, T delta_local);
bool esta_adentro( T valor,  T min,  T max);

//Enmascaramiento
CImg<bool> mascaraRectangular(unsigned int w, unsigned int h, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
CImg<unsigned char> aplicarMascara(CImg<unsigned char> & img1, CImg<bool> & mascara);
CImg<bool> getMascaraMayores(CImg<double> & imagen, double valor_min ); 

//Transformadas en espacio
CImg<unsigned char> imagenMapeo(std::vector<unsigned char> &LUT);
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img, std::vector<unsigned char> LUT);
std::vector<unsigned char> logarithmicLUT(float c);
std::vector<unsigned char> potLUT(float c, float gamma);
unsigned char truncateValue(float value);
unsigned char getLinearValue(unsigned int x, unsigned int inicio, float pendiente, float offset);
unsigned char getLogarithmicValue(unsigned int x, float c);
unsigned char getPotValue(unsigned int x, float c, float gamma);
CImg<double> get_filtro(std::string nombre);

//Color
CImg<double> falsoColor(CImg<unsigned char> img, unsigned char color[], unsigned char umbral);
template<typename T> void separarEnHSI(CImg<T> img, CImg<T> &H, CImg<T> &S, CImg<T> &I);
template<typename T> CImg<T> unirHSI(CImg<T> H, CImg<T> S, CImg<T> I);
bool dentro_circulo(double valor, double centro, double radio);
template<typename T> CImg<T> colorSlicingEsferico(CImg<T> input, T centros[], T radios[], CImg<bool> &mascara);
CImg<double> colorToBW(CImg<double> img);
template<typename T> CImg<T> cambiarIntesidad(CImg<T> imagen, CImg<T> intesidad);
bool colorSimilar(CImg<unsigned char> img, unsigned char color_comparacion[], unsigned char umbral); 
CImg<bool> getMascaraRebanado(CImg<unsigned char> img, unsigned char color[], unsigned char umbral); 

//Fourier
CImg<double> get_magnitud(CImgList<double> tdf, bool centrado);
CImg<double> get_fase(CImgList<double> tdf);
CImg<double> magn_tdf(CImg<double> image,bool centrada,const char* palname);

//Filtros frecuenciales
CImgList<double> filtroIdeal(unsigned int w, unsigned int h, unsigned int radio, bool pasaaltos);
CImgList<double> filtroButterworth(unsigned int w, unsigned int h, unsigned int frec_corte, unsigned int orden, bool pasaaltos);
CImgList<double> filtroGaussiano(unsigned int w, unsigned int h, unsigned int frec_corte, bool pasaaltos);
CImg<double> filtradoFrecuencia(CImg<double> img, CImgList<double> filtro_frec);
CImgList<double> filtroHomomorfico(unsigned int w, unsigned int h, unsigned int frec_corte, float gamma_l, float gamma_h, float c);

//Filtros de Medias
template<typename T> double mediaGeometrica(std::vector<T> vecindario);
template<typename T> double mediaContraArmonica(std::vector<T> vecindario, double Q);
template<typename T> double mediaArmonica(std::vector<T> vecindario);
template<typename T> double mediaAritmetica(std::vector<T> vecindario);
template<typename T> CImg<T> filtradoMedias(CImg<T> img, unsigned int tipo, unsigned int ancho, unsigned int alto, double Q);

//Filtros de Orden
double mediana(std::vector<double> V);
double puntoMedio(std::vector<double> V);
double alfaRecortado(std::vector<double> V, unsigned int alfa);
template<typename T> CImg<T> filtradoOrden(CImg<T> img, unsigned int tipo, unsigned int ancho, unsigned int alto, unsigned int alfa);
pair<unsigned int, unsigned int> get_index_menor_distancia(CImg<double> &vecindario, unsigned int tipo_distancia);
CImg<double> filtroMedianaGeneralizado(CImg<double> &img, unsigned int w_vecindario, unsigned int h_vecindario, unsigned int tipo_distancia);

//Filtros de bandas para ruidos
std::vector<std::pair<unsigned int, unsigned int> > leerCentros(std::string file);
CImgList<double> filtroRechazaBandaIdeal(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda);
CImgList<double> filtroRechazaBandaButterworth(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, unsigned int orden, bool pasabanda);
CImgList<double> filtroRechazaBandaGaussiano(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda);
bool dentroCirculo(int x, int y, int x0, int y0, unsigned int radio);
CImgList<double> filtroNotchAdHoc(unsigned int w, unsigned int h, std::vector<std::pair<unsigned int, unsigned int> > centros, std::vector<unsigned int> radios, bool pasante);

//Filtrado general con degradaciones o no
CImg<double> filtradoWiener(CImg<double> img, CImgList<double> filtro, double K);
CImgList<double> filtroMovimiento(unsigned int w, unsigned int h, double a, double b, double T);
CImg<double> filtradoInverso(CImgList<double> img, CImgList<double> filtro, double epsilon);

//Operadores para segmentacion
CImgList<double> operadorRoberts();
CImgList<double> operadorPrewitt();
CImgList<double> operadorSobel();
CImgList<double> operadorLaplaciano8(); 
CImgList<double> operadorLaplaciano4(); 
CImgList<double> operadorLoG(); 

//Compresion
std::vector<unsigned int> getRLC(CImg<bool> & imgbinaria); 
CImg<bool> getRLCInverse(std::vector<unsigned int> & v); 
CImg<bool> RLCCompression(CImg<double> img, unsigned int umbral, std::string filename, bool inicio); 
CImg<bool> RLCDecompression(std::string filename); 

//Segmentacion
CImg<int> label_cc(CImg<int> img, int blanco, int nueva_etiqueta);
template<typename T> CImg<int> crecimientoRegiones(CImg<T> imagen,  int seed_x,  int seed_y, T delta_local , unsigned int & pixeles_region); 

//Hough
template<typename T> std::vector<unsigned int> coordHoughToImg(CImg<T> imagen, unsigned int _tita, unsigned int _rho); 
CImg<double> hough(CImg<double> img, bool inverse); 
double houghAngulo(CImg<double> img);
double houghRho(CImg<double> img);

//=========================================================================================================================== 
//=========================================================================================================================== 
template<typename T> void printVector(std::vector<T> V) {
    for (unsigned int i = 0; i < V.size(); i++) {
        std::cout<<V[i]<<' ';
    }
    std::cout<<'\n';
}

//@ Registra los dos clicks y al tenerlos, dibuja el histograma del recuadro
template<typename T> 
void loopDisplayAndClick(CImg<T> img, std::string title = "titulo") {
    CImgDisplay ventana(img, title.c_str());   //Crea una ventana y dibuja la imagen...
    unsigned int CANTIDAD_CLICK = 0;
    //Primer y segundo click
    Click click1;
    Click click2;

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
                
                //Como ya tenemos dos clicks, estamos listos para calcular el histograma
                CImg<T> cropeada = img.get_crop(click1.x, click1.y, click2.x, click2.y);
                CImg<T> canal1 = cropeada.get_channel(0);
                CImg<T> histograma1 = canal1.get_histogram(256);
                CImg<T> histograma2;
                CImg<T> histograma3;
                if (img.spectrum() == 3) {
                    CImg<T> canal2 = cropeada.get_channel(1);
                    CImg<T> canal3 = cropeada.get_channel(2);
                    histograma2 = canal2.get_histogram(256);
                    histograma3 = canal3.get_histogram(256);
                }
            
                //Dibuja el histograma y la imagen cropeada
            
                CImgDisplay ventana2 (cropeada, "Corte");   
                histograma1.display_graph(0,3);
                if (img.spectrum() == 3) {
                    histograma2.display_graph(0,3);
                    histograma3.display_graph(0,3);
                }
                //histograma.display_graph(0,3);
            }
            CANTIDAD_CLICK++;
            if (CANTIDAD_CLICK == 2) //acotamos para evitar overflow
                CANTIDAD_CLICK = 0;
        }
    }
}

//@ Genera una imagen binaria de ladoxlado con un circulo centrado del radio pasado
CImg<bool> drawcircle(unsigned int lado, unsigned int radio) {
    CImg<bool> img(lado,lado);

    unsigned int centro = lado/2;

    cimg_forXY(img,x,y) {
        if (  (x-centro)*(x-centro) + (y-centro)*(y-centro) <= radio*radio  ) //si esta dentro del circulo
            img(x,y) = true;
        else
            img(x,y) = false;
    }
    return img;
}

//@ Devuelve el plano de bits pedido
CImg<bool> getBitPlane(CImg<unsigned char> & img1, int bit){
    CImg<bool> img(img1);
    img.fill(0);
    img = img1 & ( pow(2,bit) );
    return img;
}


//@ Crea una máscara de tamaño WxH que tiene en blanco sólo un rectangulo entre (x0,y0) y (x1,y1)
CImg<bool> mascaraRectangular(unsigned int w, unsigned int h,
                            unsigned int x0, unsigned int y0, 
                            unsigned int x1, unsigned int y1) {
    //Mascara de salida
    CImg<bool> salida(w,h,1,1,0);


    //Si los puntos vienen al reves
    if (x0 + y0 > x1 + y1) {
        //Punto 1 esta mas cerca del origen => intercambio
        unsigned int tmp;
        tmp = x0;
        x0 = x1;
        x1 = tmp;

        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    //Ponemos en true los valores dentro del rectangulo
    for (unsigned int x = x0; x < x1 && x < w; x++ ) {
        for (unsigned int y=y0; y < y1 && y < h; y++) {
            salida(x,y) = true;
        }
    }
    return salida;

}

//Enmascara una imagen con la mascara dada
CImg<unsigned char> aplicarMascara(CImg<unsigned char> & img1, CImg<bool> & mascara) {
    assert( img1.is_sameXY(mascara) );

    CImg<unsigned char> salida(img1.width(), img1.height(), 1, 1, 0);

    cimg_forXY(img1,x,y) {
        salida(x,y) = ( img1(x,y) * (unsigned int) mascara(x,y) );
    }

    return salida;
}




//@ Devuelve una imagen que representa el mapeo de LUT
CImg<unsigned char> imagenMapeo(std::vector<unsigned char> &LUT) {
    unsigned int ancho = LUT.size();
    CImg<unsigned char> mapeo(ancho, ancho,1,1,0);
    for (unsigned int i = 0; i < ancho; i++) {
        unsigned char valor = LUT[i];
        mapeo(i, ancho - valor - 1) = 255;
    }
    return mapeo;
}

//@ Aplica a img la LUT pasada por parametro
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img, std::vector<unsigned char> LUT) {
    CImg<unsigned char> resultado(img.width(), img.height());
    cimg_forXY(img, x,y) {
        resultado(x,y) = LUT[img(x,y)];
    }
    return resultado;
}

//@ Crea una LUT con la transformada logaritmo
std::vector<unsigned char> logarithmicLUT(float c) {
    std::vector<unsigned char> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = getLogarithmicValue(i, c);
    }
    return LUT;
}

//@ Crea una LUT con la transformada potencia
std::vector<unsigned char> potLUT(float c, float gamma) {
    std::vector<unsigned char> LUT;
    LUT.resize(256,0);
    for (unsigned int i = 0; i < LUT.size(); i++) {
        LUT[i] = getPotValue(i, c, gamma);
    }
    return LUT;
}


unsigned char truncateValue(float value) {
    if (value > 255) 
        return 255;
    else if (value < 0)
        return 0;
    else 
        return floor(value);
}


//Devuelve un valor acotado a 0~255
unsigned char getLinearValue(unsigned int x, unsigned int inicio, float pendiente, float offset) {
    float value = (x - inicio) * pendiente + offset;
    return truncateValue(value);
}

unsigned char getLogarithmicValue(unsigned int x, float c=1.0) {
    float value = c*log(1+x);
    float ret_val = truncateValue(value);
    return ret_val;
}

unsigned char getPotValue(unsigned int x, float c, float gamma) {
    float value = c*pow(x,gamma);
    float ret_val = truncateValue(value);
    return ret_val;
}



//@ Lee un filtro desde archivo    
CImg<double> get_filtro(std::string nombre) {
    std::ifstream f(nombre.c_str());
    if (!f.is_open()) {
        std::cout<<"No se pudo abrir el archivo "<<nombre<<"\n";
        exit(-1);
    }
    unsigned int filas;
    unsigned int columnas;
    double valor;

    f>>filas;
    f>>columnas;
    CImg<double> salida(columnas, filas, 1 , 1, 0);
    for (unsigned int i = 0; i < filas; i++) { 
        for (unsigned int j = 0; j < columnas; j++) {
            f>>valor;
            salida(j,i) =  valor;
        }
    }
    f.close();
    return salida;
}

//@ Aplica el histograma local recorriendo la imagen y ecualizando vecindarios pequenios
template<typename T>
CImg<T> histogramaLocal(CImg<T> img, unsigned int ancho = 3, unsigned int alto = 3) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            CImg<T> vecindario = img.get_crop(i - ancho/2, j - alto/2, i + ancho/2, j + alto/2);  //### ver`
            vecindario.equalize(256, 0, 255);           //primero ecualizamos
            ret_val(j,i) = vecindario(step_y,step_x);   //asigna el centro del vecindario
        }
    }
    return ret_val;
}

CImg<double> falsoColor(CImg<unsigned char> img, unsigned char color[], unsigned char umbral) {
    CImg<unsigned char> output(img.width(), img.height(), img.depth(), 3, 0);
    
    cimg_forXY(output, x, y) {
        if (img(x,y) < umbral) {
            output(x,y,0,0) = color[0]; //R
            output(x,y,0,1) = color[1]; //G
            output(x,y,0,2) = color[2]; //B
        } else {
            output(x,y,0,0) = img(x,y); //R
            output(x,y,0,1) = img(x,y); //G
            output(x,y,0,2) = img(x,y); //B
        }
    }
    return output;
}

template<typename T>
void separarEnHSI(CImg<T> img, CImg<T> &H, CImg<T> &S, CImg<T> &I) {
   CImg<T> hsi = img.get_RGBtoHSI();
   H = hsi.get_channel(0);
   S = hsi.get_channel(1);
   I = hsi.get_channel(2);
}

template<typename T>
CImg<T> unirHSI(CImg<T> H, CImg<T> S, CImg<T> I) {
    CImg<T> retval = H;
    retval.append(S,'c');
    retval.append(I,'c');
    return retval.get_HSItoRGB();
}

bool dentro_circulo(double valor, double centro, double radio) {
    return ( pow(valor-centro,2) < radio*radio );
}

template<typename T>
CImg<T> colorSlicingEsferico(CImg<T> input, T centros[], T radios[], CImg<bool> &mascara) {
    CImg<bool> mascara_binaria(input.width(), input.height());

    CImg<T> output(input.width(), input.height(), input.depth(), input.spectrum(), 0.0);

    CImg<T> c1 = input.get_channel(0);
    CImg<T> c2 = input.get_channel(1);
    CImg<T> c3 = input.get_channel(2);
    
    cimg_forXY(input, x , y) {
        if (dentro_circulo(c1(x,y), centros[0], radios[0]) &&  //rojo
            dentro_circulo(c2(x,y), centros[1], radios[1]) &&  //verde
            dentro_circulo(c3(x,y), centros[2], radios[2])) { //azul

            mascara_binaria(x,y) = true;
            
            output(x,y,0,0) = input(x,y,0,0);
            output(x,y,0,1) = input(x,y,0,1);
            output(x,y,0,2) = input(x,y,0,2);
        } else {
            mascara_binaria(x,y) = false;
        }
    }
    //Devuelve la mascara por referencia
    mascara = mascara_binaria;
    return output;
}


//Devuelve la magnitud de una transformada de Fourier    
CImg<double> get_magnitud(CImgList<double> tdf, bool centrado = false) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = sqrt( pow(tdf[0](x,y),2) + pow(tdf[1](x,y),2) );
    }

    if (centrado) {
        unsigned int w = output.width();
        unsigned int h = output.height();
        output.shift(w/2, h/2,0,0,2);
    }
    return output;
}

//Devuelve la fase de una transformada de Fourier
CImg<double> get_fase(CImgList<double> tdf) {
    CImg<double> output = tdf[0];

    cimg_forXY(output, x ,y ) {
        output(x,y) = atan2( tdf[1](x,y) , tdf[0](x,y) );
    }
    return output;
}



//@ Genera un filtro ideal con el radio especificado (frecuencia de corte)
CImgList<double> filtroIdeal(unsigned int w, unsigned int h, unsigned int radio, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    //Si es pasa altos, tenemos que iniciarla con 1, no con 0
    if (pasaaltos) {
        img[0].fill(1);
        //img[1].fill(1); //La parte imaginaria se queda en 0 igual
    }

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        if (  (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y) <= radio*radio  ) {//si esta dentro del circulo
            if (pasaaltos) img[0](x,y) = 0.0;
            else img[0](x,y) = 1.0;
        }
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}


//@ Genera un filtro de Butterworth del orden y frecuencia de corte dados
CImgList<double> filtroButterworth(unsigned int w, unsigned int h, unsigned int frec_corte, unsigned int orden, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = (1)/(1 + pow((float(frec_corte)/distancia), 2*orden));
        else img[0](x,y) =           (1)/(1 + pow((distancia/float(frec_corte)), 2*orden));
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}

//@ Genera un filtro Gaussiano
CImgList<double> filtroGaussiano(unsigned int w, unsigned int h, unsigned int frec_corte, bool pasaaltos = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    
    cimg_forXY(img[0],x,y) {
        double distancia = sqrt( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y)); 
        if (pasaaltos) img[0](x,y) = 1 - exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
        else           img[0](x,y) = exp(-pow(distancia,2)/(2*(pow(frec_corte,2))));
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}

//@ Realiza el filtrado en frecuencia a partir de la especificacion de una imagen y el filtro en frecuencia
CImg<double> filtradoFrecuencia(CImg<double> img, CImgList<double> filtro_frec) {
    
    //Obtenemos las transformadas de Fourier de la imagen y el filtro
    CImgList<double> fft_img = img.get_FFT(); 
    
    //Multiplicamos en frecuencia
    CImgList<double> tempy(2, img.width(), img.height(), img.depth(), img.spectrum(), 0);
   
    cimg_forXY(img,x,y) {
        //Capturamos los valores
        std::complex<double> factor1 (fft_img[0](x,y), fft_img[1](x,y)),
                        factor2 (filtro_frec[0](x,y), filtro_frec[1](x,y));

        //Realizamos el producto de binomios
        std::complex<double> prod = factor1*factor2;
        //Asignamos en real e imaginario
        tempy[0](x,y) = std::real(prod);
        tempy[1](x,y) = std::imag(prod);
    }
    //Calculamos la inversa
    return tempy.get_FFT(true)[0];
}


//@ Genera un filtro homomorfico
CImgList<double> filtroHomomorfico(unsigned int w, unsigned int h, unsigned int frec_corte, float gamma_l, float gamma_h, float c) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    unsigned int centro_x = w/2;
    unsigned int centro_y = h/2;
    unsigned int fcp = pow(frec_corte,2);
    
    cimg_forXY(img[0],x,y) {
        double distancia = ( (x - centro_x)*(x - centro_x) + (y - centro_y)*(y - centro_y));
        double argumento = -c * distancia/fcp;
        
        img[0](x,y) = (gamma_h - gamma_l) * (1 - exp (argumento)) + gamma_l;
    }

    img[0].shift(w/2, h/2,0,0,2);
    return img;
}

CImg<double> colorToBW(CImg<double> img) {
    if (img.spectrum() == 1) return img;
    else return img.get_RGBtoHSI().get_channel(2);
}

//@ Calcula la media geometrica para un vecindario dado
template<typename T>
double mediaGeometrica(std::vector<T> vecindario) {
    double producto = 1.0;

    //recorre el vecindario
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        producto *= vecindario[ii];
    }

    //Calcula raiz nm-esima
    producto = std::pow(producto,1.0/vecindario.size());
    return producto;
}

//@ Calcula la media Contra Armonica de un vecindario, para un valor de Q dado. Contempla los casos especiales
template<typename T>
double mediaContraArmonica(std::vector<T> vecindario, double Q) {
    //Inicializamos los contadores
    double suma_arriba = 0.0;
    double suma_abajo = 0.0;

    //Recorremos el vecindario
    for (unsigned int ii = 0; ii < vecindario.size(); ii++) {
        //Si el valor actual no es muy chico, exponencio, si no, es lo mismo que no sumar nada
        if (std::fabs(vecindario[ii]) > EPS) {
            if (Q != -1) //para cuando NO es media armonica
                suma_arriba += std::pow(vecindario[ii], Q+1);
            if (Q != 0)  //para cuando NO es media aritmetica
                suma_abajo  += std::pow(vecindario[ii], Q);
        }
    }
    
    //Normalizamos si el vecindario tenia algun valor
    if (vecindario.size() > 0) {
        suma_arriba /= (float) vecindario.size();
        suma_abajo  /= (float) vecindario.size();
    }
    
    double resultado;
    //Fix para media armonica, donde suma_arriba se hace 0
    if (Q == -1)
        resultado = 1.0;
    else
        resultado = suma_arriba;
    
    //Dividimos si suma_abajo tenia algun valor
    if (fabs(suma_abajo) > EPS)
        resultado /= suma_abajo;

    return resultado;
}

//@ Calcula la media armonica llamando a mediaContraArmonica con el parametro Q necesario
template<typename T>
double mediaArmonica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, -1);
}

//@ Calcula la media aritmetica llamando a mediaContraArmonica con el parametro Q necesario
template<typename T>
double mediaAritmetica(std::vector<T> vecindario) {
    return mediaContraArmonica(vecindario, 0);
}

//@ Aplica el filtro de medias, para un Q determinado. Llama a los otros filtros de medias segun el parametro tipo:
//tipo = 0: Media aritmetica
//tipo = 1: Media geometrica
//tipo = 2: Media armonica
//tipo = 3: media contra armonica con el parametro Q necesario
template<typename T>
CImg<T> filtradoMedias(CImg<T> img, unsigned int tipo = 0, unsigned int ancho = 3, unsigned int alto = 3, double Q = 1.0) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            //std::vector<T> vecindario = obtenerVecindario(img, i, j, w, h, step_x, step_y);
            std::vector<T> vecindario = img.get_crop(i-ancho/2, j-alto/2, i+ancho/2, j+alto/2);
            if (tipo == 0)
                ret_val(i,j) = mediaAritmetica(vecindario);
            if (tipo == 1)
                ret_val(i,j) = mediaGeometrica(vecindario);
            if (tipo == 2)
                ret_val(i,j) = mediaArmonica(vecindario);
            if (tipo == 3)
                ret_val(i,j) = mediaContraArmonica(vecindario, Q);
        }
    }
    
    return ret_val;
}

//@ Cambia el canal de intesidad de una imagen RGB, y la devuelve en RGB
template <typename T>
CImg<T> cambiarIntesidad(CImg<T> imagen, CImg<T> intesidad) {
    if (imagen.spectrum() == 1) 
        return imagen;
    
    //Convertimos a HSI
    CImg<T> img = imagen.get_normalize(0,1).get_RGBtoHSI();
    
    //Separamos los 2 canales que nos interesan
    CImg<T> canal1 = img.get_channel(0);
    CImg<T> canal2 = img.get_channel(1);

    //Componemos
    CImg<T> ret_val;
    ret_val = canal1;
    ret_val.append(canal2, 'c');
    intesidad.normalize(0,1);
    ret_val.append(intesidad, 'c');

    return ret_val.HSItoRGB().normalize(0,255);
}


//@ Devuelve la mediana de un vector calculando floor
double mediana(std::vector<double> V) {
    std::sort(V.begin(),V.end());
    return V[std::floor(V.size()/2)];
}

double puntoMedio(std::vector<double> V) {
    double maxi = *(std::max(V.begin(), V.end()));
    double mini = *(std::min(V.begin(), V.end()));
    return 0.5*(maxi + mini);
}

double alfaRecortado(std::vector<double> V, unsigned int alfa) {
    std::sort(V.begin(), V.end());
    std::vector<double> copia;
    double sum = 0.0;
    for (unsigned int i = std::floor(alfa/2); i < V.size() - std::floor(alfa/2); i++) {
        copia.push_back(V[i]);
        sum += V[i];
    }
    double ret_val = sum / double(V.size() - alfa);
    return ret_val;
}

//@ Calcula todos los filtros de orden
//Tipo 0 = Mediana
//Tipo 1 = Punto medio
//Tipo 2 = Alfa Recortado
template<typename T>
CImg<T> filtradoOrden(CImg<T> img, unsigned int tipo = 0, unsigned int ancho = 3, unsigned int alto = 3, unsigned int alfa = 4) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            std::vector<T> vecindario = obtenerVecindario(img, i, j, w, h, step_x, step_y);
            if (tipo == 0)
                ret_val(i,j) = mediana(vecindario);
            if (tipo == 1)
                ret_val(i,j) = puntoMedio(vecindario);
            if (tipo == 2)
                ret_val(i,j) = alfaRecortado(vecindario, alfa);
        }
    }
    
    return ret_val;
}

//@ Lee los centros de circulos desde un archivo
std::vector<std::pair<unsigned int, unsigned int> > leerCentros(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<std::pair<unsigned int, unsigned int> > ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int x;
        unsigned int y;
        f>>x>>y;
        ret_val.push_back(std::make_pair<unsigned int, unsigned int>(x,y));
    }
    f.close();
    return ret_val;
}

//@ Lee los radios de los circulos desde un archivo
std::vector<unsigned int> leerRadios(std::string file) {
    ifstream f (file.c_str());
    if (!f.is_open()) {
        std::cout<<"No pudo abrir el archivo "<<file<<"\n";
        exit(-1);
    }
    unsigned int cantidad;
    f>>cantidad;
    std::vector<unsigned int> ret_val;
    for (unsigned int i = 0; i < cantidad; i++) {
        unsigned int r;
        f>>r;
        ret_val.push_back(r);
    }
    f.close();
    return ret_val;
}

//@ Realiza un filtro rechaza banda ideal con ancho W y ubicado al radio especificado D0
CImgList<double> filtroRechazaBandaIdeal(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda = false) {
    //El radio es el centro de la corona. La corona tiene ancho W, y sus limites estan a W/2 del radio
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        if ((distancia < D0 - W/2) or (distancia > D0 + W/2)) {
            if (pasabanda)
                img[0](u, v) = 0.0;
            else
                img[0](u, v) = 1.0;
        }
    }

    img[0].shift(-w/2, -h/2,0,0,2);
    return img;
}

//@ Realiza un filtro rechaza banda de Butterworth con ancho W y ubicado al radio especificado
CImgList<double> filtroRechazaBandaButterworth(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, unsigned int orden, bool pasabanda = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    unsigned int D0s = D0*D0;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        double valor = (distancia * W) / (distancia * distancia - D0s);
        if (pasabanda)
            img[0](u,v) = 1 - (1 / (1 + std::pow(valor, 2*orden)));
        else
            img[0](u,v) = (1) / (1 + std::pow(valor, 2*orden));
    }

    img[0].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

//@ Realiza un filtro rechaza banda Gaussiano con ancho W y ubicado al radio especificado
CImgList<double> filtroRechazaBandaGaussiano(unsigned int w, unsigned int h, unsigned int W, unsigned int D0, bool pasabanda = false) {
    CImgList<double> img(2, w, h, 1, 1, 0);
    if (pasabanda)
        img[0].fill(1.0);

    unsigned int centro_u = w/2;
    unsigned int centro_v = h/2;
    unsigned int D0s = D0*D0;
    
    cimg_forXY(img[0],u,v) {
        double distancia = sqrt( (u - centro_u)*(u - centro_u) + (v - centro_v)*(v - centro_v));
        double valor = -0.5*std::pow((distancia*distancia - D0s) / (distancia * W), 2);
        if (pasabanda)
            img[0](u,v) = std::exp(valor);
        else
            img[0](u,v) = 1 - std::exp(valor);
    }
    
    img[0].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

bool dentroCirculo(int x, int y, int x0, int y0, unsigned int radio) {
    return std::pow(x - x0, 2) + std::pow(y - y0, 2) - radio*radio < 0;
}

//@ Realiza un filtro notch para un vector de centros y radios
CImgList<double> filtroNotchAdHoc(unsigned int w, unsigned int h, std::vector<std::pair<unsigned int, unsigned int> > centros, std::vector<unsigned int> radios, bool pasante = false) {
    //Inicializamos todo en 1
    CImgList<double> img(2, w, h, 1, 1, 1);
    if (pasante)
        img[0].fill(0); //Si es pasante, se inicializa con 0

    cimg_forXY(img[0], x, y) {
        for (unsigned int i = 0; i < centros.size(); i++) {
            if ( dentroCirculo(int(x), int(y), int(centros[i].first), int(centros[i].second), radios[i])) {
                if (pasante) //si es pasante, solo se pone en 1 si esta dentro del circulo
                    img[0](x,y) = 1.0;
                else        //si no es pasante, se pone en 0 solo en los circulos
                    img[0](x,y) = 0.0;
            }
        }
    }
    //La parte imaginaria se queda todo en 0
    img[1].fill(0.0);

    img[0].shift(-w/2, -h/2, 0, 0, 2);

    return img;
}


//@ Aplica el filtrado de Wiener para un filtro pasado por parametro
CImg<double> filtradoWiener(CImg<double> img, CImgList<double> filtro, double K) {
    unsigned int w = img.width();
    unsigned int h = img.height();
    CImgList<double> resultado(2, w, h, img.depth(), img.spectrum(), 0);
    CImgList<double> f_img = img.get_FFT();
    double re;
    double im;

    cimg_forXY(img, u, v) {
        //Para no hacer division por cero
        if (fabs(filtro[0](u,v)) < EPS)
            re = 0.0;
        else
            re = 1/filtro[0](u,v);
        //Para no hacer division por cero
        if (fabs(filtro[1](u,v)) < EPS)
            im = 0.0;
        else
            im = 1/filtro[1](u,v);
        //Componemos el primer factor 1/H
        std::complex<double> factor1 (re, im);

        //Componemos el segundo factor: H^2/(H^2 + K)
        double pot = std::pow(filtro[0](u,v), 2) + std::pow(filtro[1](u,v), 2);
        double fac2 = (pot) / (pot + K);
        std::complex<double> factor2 (fac2, 0.0);
        
        //Componemos el tercer factor: G(u,v)
        std::complex<double> factor3 (f_img[0](u,v), f_img[1](u,v));

        //Realizamos el producto de binomios
        std::complex<double> prod = factor1*factor2*factor3;

        //Asignamos en real e imaginario
        resultado[0](u,v) = std::real(prod);
        resultado[1](u,v) = std::imag(prod);
    }
    CImg<double> resultado_espacio = resultado.get_FFT(true)[0];
    return resultado_espacio;
}


//@ Devuelve un filtro de movimiento (pagina 260)
CImgList<double> filtroMovimiento(unsigned int w, unsigned int h, double a, double b, double T = 1.0) {
    CImgList<double> img(2, w, h, 1, 1, 0);

    complex<double> I(0,1);
    int inicio_x = -int(w)/2;
    int inicio_y = -int(h)/2;
    int fin_x = int(w)/2;
    int fin_y = int(h)/2;

    for (int x = inicio_x; x < fin_x; x++) {
        for (int y = inicio_y; y < fin_y; y++) {
    //for (int x = 0; x < w; x++) {
    //    for (int y = 0; y < h; y++) {
            double tempy;
            if (x == 0 and y == 0) 
                tempy = 1.0;
            else 
                tempy = 3.14159f * (double(x)*a + double(y)*b);
                //tempy = 3.14159f * (double(x-w/2)*a + double(y-h/2)*b);
          
            double val1 = T / tempy;
            double val2 = sin(tempy);
            
            unsigned int pos_x = x+w/2;
            unsigned int pos_y = y+h/2;
            
            //std::cout<<x<<' '<<y<<' '<<pos_x<<' '<<pos_y<<'\n';
            if (fabs(val2) > EPS) {
                complex<double> val3 = exp(-I * tempy);
                img[0](pos_x, pos_y) = real(val3) * val1 * val2;
                img[1](pos_x, pos_y) = imag(val3) * val1 * val2;
                //img[0](x,y) = real(val3) * val1 * val2;
                //img[1](x,y) = imag(val3) * val1 * val2;
                
                //std::cout<<tempy<<' '<<val1<<' '<<val2<<' '<<real(val3)<<' '<<imag(val3)<<'\n';
                //std::cout<<img[0](pos_x, pos_y)<<' '<<img[1](pos_x, pos_y)<<'\n';
                //std::getchar();
            }
        }
    }
    img[0].shift(-w/2, -h/2, 0, 0, 2);
    img[1].shift(-w/2, -h/2, 0, 0, 2);
    return img;
}

CImg<double> filtradoInverso(CImgList<double> img, CImgList<double> filtro, double epsilon) {
    assert(img.is_sameXY(filtro));
    CImgList<double> resultado(2,img[0].width(), img[0].height(), 1, img[0].spectrum(), 0.0);
    cimg_forXY(img[0], u, v) {
        std::complex<double> parte_img(img[0](u,v), img[1](u,v));
        std::complex<double> parte_filtro(filtro[0](u,v), filtro[1](u,v));
        if (not (fabs(real(parte_filtro)) < epsilon and fabs(imag(parte_filtro)))) { //si no son cero ambos
            std::complex<double> res = parte_img/parte_filtro;
            resultado[0](u,v) = real(res);
            resultado[1](u,v) = imag(res);
        }
    }
    return resultado.get_FFT(true)[0];
}

//@ Funcion (aux) que dado un vecindario nos devuelve la menor distancia entre c/u de los puntos
pair<unsigned int, unsigned int> get_index_menor_distancia(CImg<double> &vecindario, unsigned int tipo_distancia = 0) {

    CImg<double> tempy = vecindario;

    double acumulador;
    //Para cada uno de los elementos del vecindario
    cimg_forXY(vecindario, x, y) {
        acumulador = 0.0;
        //Comparamos contra todos los demas
        cimg_forXY(vecindario, i, j) {
            //Recorriendo cada canal
            cimg_forC(vecindario, c ) {
                double delta = vecindario(x,y,0,c) - vecindario(i,j,0,c);
                
                if (tipo_distancia == 0) {
                    //Manhattan
                    acumulador += fabs(delta);
                }
                else if (tipo_distancia == 1) {
                    //Euclidea
                    acumulador += std::pow(delta, 2);
                }
            }
        }
        if (tipo_distancia == 1) {
            //Si la distancia es euclidea, sacamos raiz cuadrada
            acumulador = std::sqrt(acumulador);
        }
        tempy(x,y) = acumulador;
        
    }

    unsigned int retx = 0;
    unsigned int rety = 0;
    acumulador = tempy(retx,rety);
    //Ahora buscamos el valor menor
    cimg_forXY(tempy,x,y) {
        if (tempy(x,y) < acumulador) {
            retx = x;
            rety = y;
            acumulador = tempy(retx,rety);
        }
    }
    // y retornamos un pair de la posicion
    return std::make_pair(retx, rety);
}

//@ Función que calcula como un filtro de mediana para espacios de mas de 1D por ejemplo para aplicar sobre imágenes a color.
CImg<double> filtroMedianaGeneralizado(CImg<double> &img, unsigned int w_vecindario = 3, unsigned int h_vecindario = 3, unsigned int tipo_distancia = 0) {
    
    CImg<double> ret_val(img.width(), img.height(), img.depth(), img.spectrum(), 0);

    cimg_forXY(img,x,y) {
        //Obtener vecindario
        CImg<double> vecindario = img.get_crop(x-w_vecindario/2, y-h_vecindario/2, x+w_vecindario/2, y+h_vecindario/2);

        //Obtener la menor distancia
        pair<unsigned int, unsigned int> coordenadas_distancia = get_index_menor_distancia(vecindario, tipo_distancia);

        //Copiamos el valor de la menor distancia
        cimg_forC(vecindario, c) { 
            ret_val(x,y,0,c) = vecindario(coordenadas_distancia.first, coordenadas_distancia.second, 0, c);
        }
    }

    return ret_val; 
}



//@ Devuelve el operador de Roberts para el gradiente en X [0] e Y [1]
CImgList<double> operadorRoberts(){
    CImgList<double> ret_val(2,2,2,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,1) = 1;

    //Gradiente en Y
    ret_val[1](0,1) = 1;
    ret_val[1](1,0) = -1;
    return ret_val;
}

//@ Devuelve el operador de Prewitt para el gradiente en las 4 direcciones
CImgList<double> operadorPrewitt(){
    CImgList<double> ret_val(4,3,3,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,0) = -1;
    ret_val[0](2,0) = -1;
    ret_val[0](0,2) = 1;
    ret_val[0](1,2) = 1;
    ret_val[0](2,2) = 1;

    ret_val[1](0,0) = -1;
    ret_val[1](0,1) = -1;
    ret_val[1](0,2) = -1;
    ret_val[1](2,0) = 1;
    ret_val[1](2,1) = 1;
    ret_val[1](2,2) = 1;

    ret_val[2](0,1) = -1;
    ret_val[2](0,2) = -1;
    ret_val[2](1,2) = -1;
    ret_val[2](1,0) = 1;
    ret_val[2](2,0) = 1;
    ret_val[2](2,1) = 1;

    ret_val[3](0,0) = -1;
    ret_val[3](0,1) = -1;
    ret_val[3](1,0) = -1;
    ret_val[3](2,2) = 1;
    ret_val[3](1,2) = 1;
    ret_val[3](2,1) = 1;

    return ret_val;
}

//@ Devuelve el operador de Sobel para el gradiente en las 4 direcciones
CImgList<double> operadorSobel(){
    CImgList<double> ret_val(4,3,3,1,1,0);
    //Gradiente en X
    ret_val[0](0,0) = -1;
    ret_val[0](1,0) = -2;
    ret_val[0](2,0) = -1;
    ret_val[0](0,2) = 1;
    ret_val[0](1,2) = 2;
    ret_val[0](2,2) = 1;

    ret_val[1](0,0) = -1;
    ret_val[1](0,1) = -2;
    ret_val[1](0,2) = -1;
    ret_val[1](2,0) = 1;
    ret_val[1](2,1) = 2;
    ret_val[1](2,2) = 1;

    ret_val[2](0,1) = -1;
    ret_val[2](0,2) = -2;
    ret_val[2](1,2) = -1;
    ret_val[2](1,0) = 1;
    ret_val[2](2,0) = 2;
    ret_val[2](2,1) = 1;

    ret_val[3](0,1) = -1;
    ret_val[3](0,0) = -2;
    ret_val[3](1,0) = -1;
    ret_val[3](1,2) = 1;
    ret_val[3](2,2) = 2;
    ret_val[3](2,1) = 1;

    return ret_val;
}

//@ Devuelve una mascara llena de -1 excepto en el centro donde tiene un 8
CImgList<double> operadorLaplaciano8() {
    CImgList<double> ret_val(1,3,3,1,1,-1);
    ret_val[0](1,1) = 8; 
    return ret_val; 
}

//@ Devuelve una mascara de 3x3 con un 4 en el medio y cuatro -1 en los extremos
CImgList<double> operadorLaplaciano4() {
    CImgList<double> ret_val(1,3,3,1,1,0);
    ret_val[0](1,1) = 4; 

    ret_val[0](0,1) = -1; 
    ret_val[0](1,0) = -1; 
    ret_val[0](1,2) = -1; 
    ret_val[0](2,1) = -1; 
    
    return ret_val; 
}

//@ Devuelve la mascara de 5x5 para el LoG
CImgList<double> operadorLoG() {
    CImgList<double> ret_val(1,5,5,1,1,0);
    ret_val[0](2,0) = -1; 
    ret_val[0](1,1) = -1; 
    ret_val[0](3,1) = -1; 
    ret_val[0](4,2) = -1; 
    ret_val[0](3,3) = -1; 
    ret_val[0](2,4) = -1; 
    ret_val[0](1,3) = -1; 
    ret_val[0](0,2) = -1; 
    
    ret_val[0](1,2) = -2; 
    ret_val[0](2,1) = -2; 
    ret_val[0](3,2) = -2; 
    ret_val[0](2,3) = -2;
    
    ret_val[0](2,2) = 16;

    return ret_val;
}

//@ Toma una lista de imagenes y le aplica el umbral especificado a cada imagen
CImgList<bool> umbralizarLista(CImgList<double> l_img, double umbral) {
    CImgList<bool> ret_val;
    //Recorre la lista
    for (unsigned int i = 0; i < l_img.size(); i++) {
        //Temporal a pushear
        CImg<bool> tempy(l_img[i].width(), l_img[i].height(), l_img[i].depth(), l_img[i].spectrum(), false);
        //Recorre la imagen
        cimg_forXY(l_img[i],x,y) {
            if (fabs(l_img[i](x,y)) > umbral) {
                tempy(x,y) = true;
            }
        }
        ret_val.push_back(tempy);
    }
    return ret_val;
}


//@ Buble sort
void BubbleSort(CImg<double> &histograma, std::vector<unsigned char> & grises_ordenados)
{
      int i, j, flag = 1;    // set flag to 1 to start first pass
      double temp;             // holding variable
      int numLength = histograma.width( ); 
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
               if (histograma(j+1, 0) > histograma(j, 0))      // ascending order simply changes to <
              { 
                    temp = histograma(j, 0);             // swap elements
                    histograma(j, 0) = histograma(j+1, 0);
                    histograma(j+1, 0) = temp;
                    flag = 1;               // indicates that a swap occurred.
                    
                    //Swap del vector
                    unsigned char tempy = grises_ordenados[j];
                    grises_ordenados[j] = grises_ordenados[j+1];
                    grises_ordenados[j+1] = tempy;
               }
          }
     }
     return;   //arrays are passed to functions by address; nothing is returned
}


//@ Realiza la codificacion RLC a partir de una imagen binaria
std::vector<unsigned int> getRLC(CImg<bool> & imgbinaria) {
    bool contando_el = 0;
    unsigned int contador = 0;

    std::vector<unsigned int> retval;

    for (unsigned int x = 0; x < imgbinaria.height(); x++ ) {
        for (unsigned int y = 0; y < imgbinaria.width(); y++ ) {
            
            if (imgbinaria(x,y) != contando_el) {
                contando_el = !contando_el;
                retval.push_back(contador);
                contador = 1;
            } else {
                contador++;
            }
        }
    }
    return retval;
}

//@ Realiza la descodificacion RLC a partir de un vector de repeticiones
//Se asume que el primer valor es el valor inicial
//El segundo valor es el ancho
//El tercer valor es el alto
//A partir del 4to estan las repeticiones, iniciando con el valor inicial especificado
CImg<bool> getRLCInverse(std::vector<unsigned int> & v) {
    bool contando_el = v[0];
    unsigned int contador = 0;
    unsigned int w = v[1];
    unsigned int h = v[2];

    CImg<bool> retval(w,h);

    unsigned int pos = 3;
    int restantes = v[pos];
    cimg_forXY(retval,x,y) {
        if (restantes == 0) {
            pos++;
            if (pos >= v.size()) {
                break;
                //std::cout<<"Error en la compresion, fuera de rango\n";
                //return retval;
            }
            else 
                restantes = v[pos]; //nuevo contador
            contando_el = !contando_el; //invertimos
        }
        retval(y,x) = contando_el;
        restantes--;
    }
    return retval;
}

CImg<bool> binarizar(CImg<unsigned int> imagen, unsigned int umbral) {
    CImg<bool> imagen_binaria(imagen.width(), imagen.height(), 1, 1, 0);
    
    //Binarizamos la imagen
    cimg_forXY(imagen, x, y) {
        if (imagen(x,y) > umbral)
            imagen_binaria(x,y) = true;
    }
    return imagen_binaria;
}

//@ Comprime una imagen escala de gris a RLC con el nombre que se le pasa, el umbral de binarizacion especificado y el valor de inicio dado
CImg<bool> RLCCompression(CImg<double> img, unsigned int umbral, std::string filename, bool inicio = false) {

    //Binarizamos
    CImg<bool> imagen_binaria = binarizar(img, umbral);

    //Obtenemos el vector a comprimir
    std::vector<unsigned int> rlc = getRLC(imagen_binaria);
    //Insertamos: [inicio, ancho, alto, ...] (se insertan al reves)
    rlc.insert(rlc.begin(), imagen_binaria.height() ); //alto
    rlc.insert(rlc.begin(), imagen_binaria.width() ); //ancho
    rlc.insert(rlc.begin(), 0 ); //comienza con

    //Guardamos el vector de compresion
    //Abre el archivo
    std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc | std::ios::out );
    //Patea si no esta abierto
    assert(file.is_open());
    //Escribe el vector entero
    file.write(reinterpret_cast<char *>(& rlc[0]), rlc.size()*sizeof(rlc[0]) );
    //Cerramos
    file.close();
    return imagen_binaria;
}

//@ Realiza todos los pasos para descomprimir una imagen a partir de un archivo con codificacion RLC
CImg<bool> RLCDecompression(std::string filename) {
    //Ahora leemos el archivo comprimido y tratamos de descomprimirlo
    //
    //Leemos
    std::ifstream f(filename.c_str(), std::ios::binary | std::ios::in );
    
    //Movemos el puntero al final para saber cuanto tamaño tiene
    f.seekg(0, std::ifstream::end);
    unsigned int tamano = f.tellg();
    //Lo volvemos al principio para hacer una lectura secuencial
    f.seekg(0, std::ifstream::beg);

    //Creamos el vector que leera la imagen comprimida, del tamaño que nos dio el paso anterior
    std::vector<unsigned int> lectura;
    lectura.resize(tamano/(sizeof(unsigned int)));
    
    //Lee el contenido entero del archivo al vector
    f.read((char *) &lectura.front(), lectura.size()*sizeof(lectura.front()));
    //Cerrramos el archivo
    f.close();
   
    //A partir del vector, reconstruimos la imagen
    CImg<bool> img_leida;
    img_leida = getRLCInverse(lectura);
    return img_leida;
}


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



CImg<bool> getMascaraRebanado(CImg<unsigned char> img, unsigned char color[], unsigned char umbral) {
    CImg<bool> ret_val(img.width(), img.height(), 1, 1, true);
    cimg_forXY(img, x, y) {
        if (colorSimilar(img.get_crop(x,y,x,y), color, umbral)) {
            ret_val(x,y) = false;
        }
    }
    return ret_val;
}

template<typename T>
std::vector<unsigned int> coordHoughToImg(CImg<T> imagen, unsigned int _tita, unsigned int _rho) {
    unsigned int M = imagen.width();
    unsigned int N = imagen.height();

    double step_tita = M_PI/double(M-1.0);
    double diagonal = std::sqrt(std::pow(M-1,2) + std::pow(N-1,2));
    double step_rho = 2.0*diagonal/double(N-1);
    //Actualiza tita y rho a valores reales
    double tita = double(_tita)*step_tita - M_PI/2.0; 
    double rho  = double(_rho)*step_rho - diagonal;

    unsigned int x0;
    unsigned int x1;
    unsigned int y0;
    unsigned int y1;
    
    if (tita>-M_PI/2 && tita<M_PI/2){
        y0 = 0; 
        y1 = M-1;
        x0 = rho/cos(tita);      // calcula y para y=0
        x1 = rho/cos(tita)-(M-1)*tan(tita); // calcula y para y=M-1   
    }else{
        x0 = 0; 
        x1 = N-1;
        y0 = rho/sin(tita);      // calcula y para x=0
        y1 = rho/sin(tita)-(N-1)/tan(tita); // calcula y para x=M-1
    }
    unsigned int ret_val[4] = {y0,x0,y1,x1};
    std::vector<unsigned int> retval;
    retval.assign(&ret_val[0], &ret_val[0]+4);
    return retval;
}

//@ Devuelve el angulo en grados donde se halla el mayor
double houghAngulo(CImg<double> img) {
    double maxi = img.max();
    unsigned int tita;
    cimg_forXY(img,x,y) {
        if (fabs(img(x,y) - maxi) < EPS) {
            tita = x;
            break;
        }
    }
    double angulo = (180.0 / double(img.width())) * double(tita) - 90.0;
    return angulo;
}

//@ Devuelve el rho del mayor valor, en distancia imagen
double houghRho(CImg<double> img) {
    double maxi = img.max();
    unsigned int _rho;
    cimg_forXY(img,x,y) {
        if (fabs(img(x,y) - maxi) < EPS) {
            _rho = y;
            break;
        }
    }
    double diag = sqrt(pow(img.width(),2) + pow(img.height(),2));
    double rho = ((2.0*diag) / double(img.height()))*double(_rho) - diag;
    return rho;
}


// Devuelve si un valor está dentro del rango (min, max)
template<typename T>
bool esta_adentro( T valor,  T min,  T max) {
    if (min > max) {
        T tempay = min;
        min = max;
        max = tempay;
    }   
    return (valor < max) && (valor > min || fabs(valor - min) < 0.00001 );
}


//***** bool es_deseada *****
// Dado un píxel de referencia y uno a evaluar
// los compara independientemente de la cantidad de canales que tenga
// en base a un umbral delta_local

// ---------Entradas------
// v_evaluar        : pixel a evaluar. Se puede obtener con imagen.get_crop(x,x,y,y)
// v_referencia    :  pixel de referencia
// delta_local : umbral de variacion admisible pixel a pixel canal a canal
// ---------Salidas------
// verdadero si cumple con la condición
template<typename T>
bool es_deseada(CImg<T> v_evaluar, CImg<T> v_referencia, T delta_local) {
    assert( v_referencia.size() == v_evaluar.size() && v_referencia.spectrum() == v_evaluar.spectrum() );
    bool deseada = true;
    cimg_forC(v_evaluar, c) {
        T rango_superior = v_referencia(0,0,0,c) + delta_local;
        if (rango_superior > 255)
            rango_superior = 255; 
        
        T rango_inferior = v_referencia(0,0,0,c) - delta_local;
        if (rango_inferior < 0)
            rango_inferior = 0; 

        deseada = deseada && esta_adentro(v_evaluar(0,0,0,c), rango_inferior , rango_superior + 1 );
    }
    return deseada;
}



//Estructura necesaria para poder hacer ordenamiento de los mayores N pixeles en por ejemplo una T de hough
struct pixel {
    unsigned int x;
    unsigned int y;
    double valor;
};

bool pixel_es_mayor(pixel a, pixel b) { return a.valor > b.valor; }


std::vector<pixel> getNMayores(CImg<double>  & though , unsigned int cantidad) {
    std::vector<pixel> v;

    cimg_forXY(though,x, y) {
        pixel t;
        t.x = x;
        t.y = y;
        t.valor = though(x,y);
        v.push_back(t);
    }
    std::sort(v.begin(), v.end(), pixel_es_mayor);

    v.erase(v.begin() + cantidad );
    return v;
}




//***** Crecimiento de regiones para color *****
// Se fija en los 3 canales para decidir si un pixel pertenece 
// a la region inicializada por la semilla.

// ---------Entradas------
// imagen
// seed_x       : coordenada semilla
// seed_y       : coordenada semilla
// delta_local : umbral de variacion admisible pixel a pixel canal a canal
// ---------Salidas------
// Devuelve una imagen con 3 valores
//   0: pixel no pertenece a la region
//   1: pixel que pertenece a la region
//  -1: pixel no procesado

template<typename T>
CImg<int> crecimientoRegiones(CImg<T> imagen,  int seed_x,  int seed_y, T delta_local , unsigned int & pixeles_region) {
    pixeles_region = 0;
    //Imagen temporal para saber que pixeles han sido procesados
    CImg<int> retval(imagen.width(), imagen.height(), 1, 1, -1 );

    //Cola a procesar
    std::queue< std::vector< int> > a_procesar;
    std::vector<int> coordtemp; coordtemp.push_back(seed_x); coordtemp.push_back(seed_y);
    a_procesar.push( coordtemp );

    //El primer pixel, pertenece a la region
    retval(seed_x,seed_y) = 1;
    pixeles_region++;

    while(! a_procesar.empty() ) {
        //Guarda el primero de la cola
        std::vector< int > procesando = a_procesar.front();
        //Borra el primero de la cola
        a_procesar.pop();
        //Recorremos el vecindario de 8 vecinos a la cola
        for ( int i = -1; i < 2; i++) {
            for ( int j = -1; j < 2; j++) {
                if (
                    esta_adentro(procesando[0] + i, 0 , imagen.width() ) && //en rango x
                    esta_adentro(procesando[1] + j, 0 , imagen.height() ) &&  //en rango y
                    retval(procesando[0] + i, procesando[1] + j) == -1 )  {  //no procesado
                        //caso esta en rango y No ha sido procesada
                        if ( es_deseada(
                                imagen.get_crop(
                                    procesando[0] + i, procesando[1] + j, procesando[0] + i, procesando[1] + j
                                ), 
                                imagen.get_crop(
                                    procesando[0], procesando[1], procesando[0], procesando[1]
                                    ),
                                    delta_local
                                ) 
                            ) {

                            retval(procesando[0] + i, procesando[1] + j) = 1;
                            pixeles_region++;
                            coordtemp.clear(); coordtemp.push_back(procesando[0] + i); coordtemp.push_back( procesando[1] + j);
                            a_procesar.push(coordtemp);
                        } else {
                            retval(procesando[0] + i, procesando[1] + j) = 0;
                        }
                    }
            }
        }
    }
    return retval;
}


template <typename T>
unsigned int contarValor(CImg<T> imagen, T valor) {
    assert(imagen.spectrum() == 1);
    unsigned int contador = 0;
    cimg_forXY(imagen, x, y) {
        if (fabs(valor-imagen(x,y)) < 0.00001) {
            contador++;
        }
    }
    return contador;
}



CImg<bool> getMascaraMayores(CImg<double> & imagen, double valor_min ) {

    CImg<double> retval(imagen.width(), imagen.height(), 1, 1, 0);

    cimg_forXY(imagen, x, y) {
        if (imagen(x,y) > valor_min) {
            retval(x,y) = 1;
        }
    }

    return retval;

}

void getPuntosCardinales(CImg<double> img, std::vector<unsigned int> & cardinalidades, double umbral = 0.1 ) {
    cardinalidades.clear();
    img.normalize(0,1);

    unsigned int W = img.width();
    unsigned int H = img.height();

    int i = 0;
    int j = 0;

    //Encontramos el Norte
    for (j = 0; j < H; ++j) {
        for (i = 0; i < W; ++i) {
            if (img(i,j) > umbral) {
                    if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
                break;
        }
        }
    }
    cardinalidades.push_back(i);
    cardinalidades.push_back(j);

    //Encontramos el Sur
    for (j = H; j >= 0; --j) {
        for (i = 0; i < W; ++i) {
            if (img(i,j) > umbral) {
                    if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
                break;
        }
        }
    }
    cardinalidades.push_back(i);
    cardinalidades.push_back(j);

    //Encontramos el Este
    for (i = W; i >= 0; --i) {
        for (j = 0; j < H; ++j) {
            if (img(i,j) > umbral) {
                    if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
                break;
        }
        }
    }
    cardinalidades.push_back(i);
    cardinalidades.push_back(j);

    //Encontramos el OEste
    for (i = 0; i < W; ++i) {
        for (j = 0; j < H; ++j) {
            if (img(i,j) > umbral) {
                    if (img(i,j) > umbral); cout<<img(i,j)<<"\n";
                break;
        }
        }
    }
    cardinalidades.push_back(i);
    cardinalidades.push_back(j);

}

///****************************************
/// Transformada Hough directa e inversa
///****************************************
CImg<double> hough(CImg<double> img, bool inverse=false) {

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
          r=floor((rho+max_rho)/step_rho+.5); // mapea r en [-max_rho , max_rho] a r en [0,N-1] el floor(r+.5) sirve para redondear
          iHough(t,r)+= 1;               // suma el acumulador     
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

///****************************************
/// Cargar paleta
///****************************************
CImg<float> cargar_paleta(const char* filename="../../paletas/gray.pal"){
  CImg<float> paleta(1,256,1,3,0);
  
  FILE *fichero;
  float x0, x1, x2;
  int i, leidos;
  
  fichero = fopen( filename, "r" );
  
  for( i=1; i<=256; i++ )
  {
    leidos=fscanf( fichero, "%e\t%e\t%e\n", &x0, &x1, &x2);
    if (leidos){
      paleta(0,i-1,0,0)=x0;
      paleta(0,i-1,0,1)=x1;
      paleta(0,i-1,0,2)=x2;
    }
  }
  
  return paleta;
}



///****************************************
/// Magnitud de la TDF transformada logaritmicamente
///****************************************
CImg<double> magn_tdf(CImg<double> image,bool centrada=true,const char* palname="../images/paletas/hot.pal"){
  CImgList<double> tf = image.get_FFT(false);  // lista con 2 imagenes: parte real e imaginaria
  CImg<double> preal = tf[0], 
               pimag = tf[1]; // partes separadas
  int nn = preal.width();
  int mm = preal.height();
  CImg<double> magnitud(nn,mm), 
               magnitud_paleta(nn,mm,1,3,0);
  for (int i = 0; i < nn; i++)
    for (int j = 0; j < mm; j++) 
      magnitud(i,j) = log (sqrt( pow(double(preal(i,j)),2) + 
                                 pow(double(pimag(i,j)),2.)
                               ) 
                                 + 0.000001
                          );
  magnitud.normalize(0,255);
  
  if (centrada)
    (magnitud).shift(magnitud.width()/2,magnitud.height()/2,0,0,2); //parametros de scroll:x,y,z,v,border_condition

  //DEVUELVE LA TDF CON LA PALETA DESEADA
  CImg<float> paleta=cargar_paleta(palname);
  
  for (int i=0; i<nn; i++) {
    for (int j=0; j<mm; j++) {
      magnitud_paleta(i,j,0,0)=255*paleta(0,int(floor(magnitud(i,j))),0,0);
      magnitud_paleta(i,j,0,1)=255*paleta(0,int(floor(magnitud(i,j))),0,1);
      magnitud_paleta(i,j,0,2)=255*paleta(0,int(floor(magnitud(i,j))),0,2);
    }
  } 
  
  return magnitud_paleta;
}


///****************************************
/// Etiquetado de componentes conectadas
///****************************************
CImg<int> label_cc(CImg<int> img, int blanco=1, int nueva_etiqueta=2){
 
  vector<int> equiv(nueva_etiqueta+1,0); //vector de equivalencias
  vector<int> vecinos;                   //vector de etiquetas vecinos superiores e izquierda
  int pos, etiqueta, aux;

  cimg_forXY(img,x,y){           // recorro la imagen
    if (img(x,y)==blanco){       // si es blanco
      vecinos.clear();           // inicializo 
      if (x && y)                // si x no es borde izq e y no es borde superior miro el vecino sup izq
        if (img(x-1,y-1)!=0)  vecinos.push_back(img(x-1,y-1)); // si tiene etiqueta la guardo
      if (y)                     // si y no es borde superior miro vecino superior
        if (img(x,y-1)!=0)    vecinos.push_back(img(x,y-1));   // si tiene etiqueta la guardo
      if (y && x!=img.width()-1)  // si x no es borde derecho e y borde superior miro vecino sup der
        if (img(x+1,y-1)!=0)  vecinos.push_back(img(x+1,y-1)); // si tiene etiqueta la guardo
      if (x)                     // si x no es borde izquierdo miro vecino izq
        if (img(x-1,y)!=0)    vecinos.push_back(img(x-1,y));   // si tiene etiqueta la guardo
      if (vecinos.empty()) {     // si no tengo vecinos etiquetados debo generar nueva etiqueta
        vecinos.push_back(nueva_etiqueta); // de vecinos voy a sacar la etiqueta que corresponde al pixel
        equiv[nueva_etiqueta]=nueva_etiqueta; // guardo en la posicion nva etiqueta el valor nva etiqueta
        nueva_etiqueta++;        // genero una nueva etiqueta para cdo necesite
        equiv.push_back(0);      // agrego una posicion en equivalencias con 0
      } else {
        for (int i=0; i<vecinos.size()-1; i++)  // controlo la lista de etiquetas vecinas
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

  // Muestro como quedo la tabla
  cout<<"Tabla de equivalencias"<<endl<<endl;
  for (int j=0; j<equiv.size(); j++)
    cout<<j<<" "<<equiv[j]<<endl;
  cout<<endl;

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
