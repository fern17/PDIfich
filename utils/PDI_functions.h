/***************************************************************************
 *            PDI_functions
 *
 *  2012 / 04 / 04
 *  Cesar Martinez - cmartinez@bioingenieria.edu.ar
 *  Monica Larese  - mlarese@ifir.edu.ar
 *  Omar Müller - ovmuller@unl.edu.ar
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <CImg.h>
using namespace std;
using namespace cimg_library;
///****************************************
///        Definiciones generales
///****************************************
#define MAX_LINE_LENGTH 16384
char linea[MAX_LINE_LENGTH];
const int NFilas256=256, NColumnas256=256;

///****************************************
/// Crecimiento de regiones
///****************************************
CImg<unsigned char> region_growing(CImg<unsigned char> orig,int x,int y,int delta, int etiqueta){
  // orig: imagen a procesar
  // x,y: posición de la semilla
  // delta: define el rango de pertenencia como [semilla-delta/2, semilla+delta/2]
  // etiqueta: nro de la etiqueta, no debe pertenecer al rango
  
  //Basicamente pinto la semilla y la pongo en la cola
  //despues tomo el primer nodo de la cola como referencia (a su vez lo elimino de la cola),
  // miro sus vecinos, los pinto y los agrego al final de la cola

  struct node{
    int x;
    int y;
  }node;
  // armo el rango
  int valor=orig(x,y);
  int rango_min=valor-delta/2,
      rango_max=valor+delta/2+delta%2; //para delta impar suma 1 para delta par suma 0
  if (rango_min<0) rango_min=0;
  if (rango_max>255) rango_max=255;
  
  //muestro en pantalla y controlo etiqueta
  cout<<"semilla: "<<valor<<" rango: ["<<rango_min<<","<<rango_max<<"] etiqueta: "<<etiqueta<<endl;
  if (etiqueta>=rango_min && etiqueta<=rango_max){
    cout<<"error: la etiqueta ("<<etiqueta<<") no puede pertenecer al rango, vuelva a elegir"<<endl;
    cout<<"etiqueta: ";
    cin>>etiqueta;
  }  


  CImg<> dest(orig);
  vector<int> camino;

  dest(x,y)=etiqueta;
  //agrego el nodo (x,y) al final de la cola sólo por una cuestion de inicializacion
  //para que el erase del while tenga sentido en la primer ejecucion
  camino.push_back(x);
  camino.push_back(y);
  //mientras exista cola
  while(camino.size()!=0){
//     cout<<camino.size()<<" ";
    //tomo los dos primeros valores de la cola
    node.x=camino[0];
    node.y=camino[1];

    //elimino el nodo de la cola xq ya lo tengo pintado
    camino.erase(camino.begin(),camino.begin()+2); 

    //miro a la derecha
    if ((node.x+1 > 0) && (node.x+1 < dest.width()))
      if ((node.y > 0) && (node.y < dest.height()))
        if (dest(node.x+1,node.y) >= rango_min && dest(node.x+1,node.y) <= rango_max){
          //si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
          dest(node.x+1,node.y)=etiqueta;
          camino.push_back(node.x+1);
          camino.push_back(node.y);
        }
    //miro a la izquierda
    if ((node.x-1 > 0) && (node.x-1 < dest.width()))
      if ((node.y > 0) && (node.y < dest.height()))
        if (dest(node.x-1,node.y) >= rango_min && dest(node.x-1,node.y) <= rango_max){
          //si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
          dest(node.x-1,node.y)=etiqueta;
          camino.push_back(node.x-1);
          camino.push_back(node.y);
        }
    //miro abajo
    if ((node.x > 0) && (node.x < dest.width()))
      if ((node.y+1 > 0) && (node.y+1 < dest.height()))
        if (dest(node.x,node.y+1) >= rango_min && dest(node.x,node.y+1) <= rango_max){
          //si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
          dest(node.x,node.y+1)=etiqueta;
          camino.push_back(node.x);
          camino.push_back(node.y+1);
        }
    //miro arriba
    if ((node.x > 0) && (node.x < dest.width()))
      if ((node.y-1 > 0) && (node.y-1 < dest.height()))
        if (dest(node.x,node.y-1) >= rango_min && dest(node.x,node.y-1) <= rango_max){
          //si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
          dest(node.x,node.y-1)=etiqueta;
          camino.push_back(node.x);
          camino.push_back(node.y-1);
        }
  }
  return dest;
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
  img.display("Primera Pasada");

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
/// Filtro ideal
///****************************************
void generar_filtro_ideal256(float m[][NFilas256], int R) {
  for (int i=0; i<NColumnas256; i++)
    for (int j=0; j<NFilas256; j++)
      if (sqrt(pow(double(i-NColumnas256/2),2)+pow(double(j-NFilas256/2),2))<R)
	m[i][j]=1;
      else m[i][j]=0;
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
  CImgList<double> tf=image.get_FFT(false);  // lista con 2 imagenes: parte real e imaginaria
  CImg<double> preal=tf[0], pimag=tf[1]; // partes separadas
  int nn=preal.width();
  int mm=preal.height();
  CImg<double> magnitud(nn,mm), magnitud_paleta(nn,mm,1,3,0);
  for (int i=0; i<nn; i++)
    for (int j=0; j<mm; j++) 
      magnitud(i,j)=log(sqrt(pow(double(preal(i,j)),2)+pow(double(pimag(i,j)),2.))+0.000001);
  magnitud.normalize(0,255);
  if (centrada)
    (magnitud).shift(magnitud.width()/2,magnitud.height()/2,0,0,2); //parametros de scroll:x,y,z,v,border_condition

  //DEVUELVE LA TDF CON LA PALETA DESEADA
  CImg<float> paleta=cargar_paleta(palname);
  for (int i=0; i<nn; i++){
    for (int j=0; j<mm; j++) {
      magnitud_paleta(i,j,0,0)=255*paleta(0,int(floor(magnitud(i,j))),0,0);
      magnitud_paleta(i,j,0,1)=255*paleta(0,int(floor(magnitud(i,j))),0,1);
      magnitud_paleta(i,j,0,2)=255*paleta(0,int(floor(magnitud(i,j))),0,2);
    }
  }	
  return magnitud_paleta;
}


///**********************************************************************
/// Muestra una imagen en 3D, tomando la intensidad como la componente z
/// Advertencia! El eje z aparece invertido
///**********************************************************************
void draw_3D_image(CImg<unsigned char> imagen, const float sigma=1.0f, const float ratioz=1.f, const unsigned int di=10){
  // Init data
  const CImg<unsigned char> img = imagen.blur(sigma).resize(-100,-100,1,3);
  const CImg<unsigned char> norm = img.get_norm().normalize(0,255);
   
  CImgList<unsigned int> primitives;
  CImgList<unsigned char> colors;
  const CImg<> points = img.get_elevation3d(primitives,colors,norm*-ratioz);
   
  // Compute image isophotes.
  std::fprintf(stderr,"\n- Compute image isophotes"); std::fflush(stderr);
  CImgList<unsigned int> isoprimitives;
  CImgList<unsigned char> isocolors;
  CImg<> isopoints;
   
  for (unsigned int i = 0; i<255; i+=di) {
    CImgList<> prims;
    const CImg<> pts = norm.get_isoline3d(prims,(float)i);
    isopoints.append_object3d(isoprimitives,pts,prims);
  }   
  cimglist_for(isoprimitives,l) {
    const unsigned int i0 = isoprimitives(l,0);
    const float x0 = isopoints(i0,0), y0 = isopoints(i0,1);
    const unsigned char
    r = (unsigned char)img.linear_atXY(x0,y0,0),
    g = (unsigned char)img.linear_atXY(x0,y0,1),
    b = (unsigned char)img.linear_atXY(x0,y0,2);
    isocolors.insert(CImg<unsigned char>::vector(r,g,b));
  }
  cimg_forX(isopoints,ll) isopoints(ll,2) = -ratioz*norm.linear_atXY(isopoints(ll,0),isopoints(ll,1));
  
  // Enter event loop
  std::fprintf(stderr,"\n- Enter interactive loop.\n\n"
		      "Reminder : \n"
		      " + Use mouse to rotate and zoom object\n"
		      " + key 'F' : Toggle fullscreen\n"
		      " + key 'Q' or 'ESC' : Quit\n"
		      " + Any other key : Change rendering type\n\n"); std::fflush(stderr);
  const char *const title = "Image viewed as a surface";
  CImgDisplay disp(800,600,title,0);
  unsigned int rtype = 2;
  CImg<float> pose = CImg<float>::identity_matrix(4);
   
  while (!disp.is_closed()) {
    const unsigned char white[3]={ 255, 255, 255 };
    CImg<unsigned char> visu(disp.width(),disp.height(),1,3,0);
    visu.draw_text(10,10,"Render : %s",white,0,1,19,rtype==0?"Points":(rtype==1?"Lines":(rtype==2?"Faces":(rtype==3?"Flat-shaded faces":
	 (rtype==4?"Gouraud-shaded faces":(rtype==5?"Phong-shaded faces":"Isophotes"))))));
    if (rtype==6) visu.display_object3d(disp,isopoints,isoprimitives,isocolors,true,1,-1,true,500.0f,0.0f,0.0f,-5000.0f,0.0f,0.0f,true,pose.data());
    else visu.display_object3d(disp,points,primitives,colors,true,rtype,-1,true,500.0f,0.0f,0.0f,-5000.0f,0.0f,0.0f,true,pose.data());
    switch (disp.key()) {
      case 0: break;
      case cimg::keyBACKSPACE: rtype = (7+rtype-1)%7; break;
      case cimg::keyQ:
      case cimg::keyESC: disp.close(); break;
      case cimg::keyF:
      if (disp.is_fullscreen()) disp.resize(800,600); else disp.resize(disp.screen_width(),disp.screen_height());
      disp.toggle_fullscreen();
      break;
      default: rtype = (rtype+1)%7; break;
    }
  } 
}


