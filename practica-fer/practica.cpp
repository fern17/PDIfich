#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace cimg_library;   //Necesario
using namespace std;


/* NELLMELDIN, FERNANDO. 34.565.066
 * PASOS REALIZADOS
 * 1) Para identificar la rotacion del billete, se propone examinar la parte blanca del billete (a la izquierda).
 * Primero se convirtio a HSI y se trabajo solo con el canal de intensidad.
 * Si la media del rectangulo de la izquierda, es menor a un umbral establecido, entonces esa parte esta rotada, porque si no deberia ser blanca y su media muy alta (mayor al umbral)
 * 
 * 2) Identificacion de billete: Se propone examinar la region de los rombos, ya que la cantidad de rombos denota el tipo de billete. Para eso, primero habria que segmentarlos.
 * Se recorta la imagen para analizar solo la region de los rombos. 
 * *****Esta es la principal falencia del metodo, ya que la region se identificó empiricamente y un leve movimiento del billete puede dar malos resultados. 
 * *****Por lo tanto, se analiza una region un poco mas grande que la que abarcan los rombos, de manera de contemplar leves movimientos.
 * 
 * Se le pasa una umbralizacion de manera de quedarnos con una imagen binaria, cuyo fondo es blanco y los rombos son negros
 * 
 * Como puede haber ruido ocasionado en la umbralizacion, se pasa la imagen binaria por un filtro de mediana de talla 5, para que elimine las impurezas
 * 
 * Luego se realiza un etiquetado de componentes conexas para contar la cantidad de rombos
 * 
 * Al finalizar el etiquetado, se cuenta la cantidad de componentes. Para eso, se recorre la imagen y se inserta su valor en un std::set<int>. 
 *  Si el valor esta repetido, no lo inserta en el set (porque tiene valores unicos almancenados)
 * 
 * Para contar la cantidad de rombos finales, se calcula el tamaño del set-1, ya que hay una region extra que es el fondo.
 * 
 * Para finalizar, como ya sabemos la cantidad de rombos, comparamos eso con los valores de cada billete:
 * 6 rombos => billete de 2
 * 5 rombos => billete de 5
 * 4 rombos => billete de 10
 * 3 rombos => billete de 20
 * 2 rombos => billete de 50
 * 1 rombo => billete de 100
*/
//Devuelve true si hay que rotar la imagen, basada en la media de una region blanca de la izquierda
bool identificarRotacion(CImg<double> img, double umbral){ 
    unsigned int h = img.height();
    unsigned int w = img.width();
    //pasamos a nivel de gris
    CImg<double> gris = img.get_RGBtoHSI().get_channel(2);
    //recortamos la parte de interes
    CImg<double> vecindario = gris.get_crop(0,0,180,h);
    //calculamos la media
    double media = vecindario.mean();
    std::cerr<<"media = "<<media<<'\n';
    if (media > umbral) return false;
    else return true;
}

//@ Calcula todos los filtros de orden
template<typename T>
CImg<T> filtradoOrden(CImg<T> img, unsigned int ancho = 3, unsigned int alto = 3, unsigned int alfa = 4) {

    CImg<T> ret_val = img;
    unsigned int h = img.height(); 
    unsigned int w = img.width();  
    unsigned int step_x = ancho/2;              //ancho del vecindario centrado
    unsigned int step_y = alto/2;               //alto del vecindario centrado
    
    for (unsigned int i = 0; i < img.width(); i++) {
        for (unsigned int j = 0; j < img.height(); j++) {
            //recortamos el vecindario
            CImg<T> vecindario = img.get_crop(i-ancho/2,j-alto/2,i+ancho/2,j+alto/2);
            //ordenamos el vecindario de mayor a menor
            vecindario.sort();
            //nos quedamos con la mediana (valor del centro)
            T median = vecindario(vecindario.width()/2, vecindario.height()/2);
            ret_val(i,j) = median;
        }
    }
    return ret_val;
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
  /*cout<<"Tabla de equivalencias"<<endl<<endl;
  for (int j=0; j<equiv.size(); j++)
    cout<<j<<" "<<equiv[j]<<endl;
  cout<<endl;
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

int main(int argc, char *argv[]) {
 	const unsigned char par1 = cimg_option("-j", 6, "p1");
    const double umbral = cimg_option("-u", 0.8, "Umbral para detectar que el billete esta dado vuelta");
    const double umbral_rombos = cimg_option("-r", 0.7, "Umbral para detectar rombos");

    std::vector<std::string> archivos;

    //Carga de todos los casos de prueba
    archivos.push_back("img/B2C1_01.jpg");
    archivos.push_back("img/B2C1_02.jpg");
    archivos.push_back("img/B2C1_03a.jpg");
    archivos.push_back("img/B2C1_04a.jpg");
    archivos.push_back("img/B5C1_01.jpg");
    archivos.push_back("img/B5C1_02.jpg");
    archivos.push_back("img/B5C1_03a.jpg");
    archivos.push_back("img/B5C1_06a.jpg");
    archivos.push_back("img/B10C1_01.jpg");
    archivos.push_back("img/B10C1_02.jpg");
    archivos.push_back("img/B10C1_03a.jpg");
    archivos.push_back("img/B10C1_04a.jpg");
    archivos.push_back("img/B20C1_01.jpg");
    archivos.push_back("img/B20C1_02.jpg");
    archivos.push_back("img/B20C1_03a.jpg");
    archivos.push_back("img/B20C1_04a.jpg");
 	archivos.push_back("img/B50C1_01.jpg");
 	archivos.push_back("img/B50C1_02.jpg");
 	archivos.push_back("img/B50C1_03a.jpg");
 	archivos.push_back("img/B50C1_04a.jpg");
 	archivos.push_back("img/B100C1_01.jpg");
 	archivos.push_back("img/B100C1_02.jpg");
 	archivos.push_back("img/B100C1_03a.jpg");
 	archivos.push_back("img/B100C1_04a.jpg");

 	for (int i = 0; i < archivos.size(); i++){
        CImg<unsigned char> img(archivos[i].c_str());
        unsigned int h = img.height();
        unsigned int w = img.width();
        CImg<double> rotada = img;
        //Identificacion de la rotacion segun la media de un rectangulo grande de la izquierda del billete
        if (identificarRotacion(img, umbral)) {
            std::cerr<<"Hay que rotarla\n";
            rotada = img.get_rotate(180,w/2,h/2);
        }
        //Trabajaremos solo con el canal I de HSI
        CImg<double> ii = rotada.get_RGBtoHSI().get_channel(2);
        
        //Recortamos la region correspondiente a los rombos
        //CImg<double> rombos = ii.get_crop(125,10,180,75); //esta anda bien
        CImg<double> rombos = ii.get_crop(120,10,185,80); //un poco mas grande, pero es mas sensible al borde con ruido derecho, por eso la mediana se debe hacer mas grande
        
        //Umbralizamos el billete para quedarnos con una imagen binaria (negro = rombo, blanco = fondo)
        CImg<bool> rombos_umbral(rombos.width(), rombos.height(), 1, 1, false);
        cimg_forXY(rombos,x,y) {
            if (rombos(x,y) < umbral_rombos)
                rombos_umbral(x,y) = true;
        }
        
        //Aplicamos un filtrado de mediana con talla grande para eliminar impurezas
        //CImg<bool> rombos_filtrado = filtradoOrden(rombos_umbral,5,5); //una mediana chica para el crop mas chico
        CImg<bool> rombos_filtrado = filtradoOrden(rombos_umbral,11,11); //una mediana grande para el crop grande (agrega mas pixeles y por lo tanto puede haber ruido)
        
        //Realizamos etiquetado de componentes conexas para contar la cantidad de regiones
        CImg<int> etiquetado = label_cc(rombos_filtrado);
        
        //Contamos la cantidad de valores distintos. std::set<int> solo almacena valores distintos
        std::set<int> diferentes;
        cimg_forXY(etiquetado,x,y){
            diferentes.insert(etiquetado(x,y)); //si ya estaba, el insert no tiene efecto
        }
        
        //Contamos la cantidad de regiones. Hay una region extra que es el fondo, por eso se resta
        int cantidad_rombos = diferentes.size()-1;
        std::cerr<<"Cantidad de rombos = "<<cantidad_rombos<<'\n';
        
        //Ahora clasificamos segun la cantidad de rombos (uso cerr porque asi no tengo que esperar a que se llene el buffer del cout para que escriba)
        if (cantidad_rombos == 6) 
            std::cerr<<"Billete de 2\n";
        else if (cantidad_rombos == 5) 
            std::cerr<<"Billete de 5\n";
        else if (cantidad_rombos == 4) 
            std::cerr<<"Billete de 10\n";
        else if (cantidad_rombos == 3) 
            std::cerr<<"Billete de 20\n";
        else if (cantidad_rombos == 2) 
            std::cerr<<"Billete de 50\n";
        else if (cantidad_rombos == 1) 
            std::cerr<<"Billete de 100\n";
        //Dibujamos
        (img, rotada, rombos_filtrado).display();
        //(rombos_filtrado).display();
 	}

    return 0;
}
