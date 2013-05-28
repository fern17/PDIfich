/******************************************************
 *            lossy_rlc
 *
 *  Fri Jun 1 2007
 *  Copyright  2007  Mónica Larese
 *  mlarese@ifir.edu.ar
 *******************************************************/

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

#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <CImg.h>
#include <sys/stat.h>
#include <time.h>
using namespace std;
using namespace cimg_library;

int main(int argc, char** argv) {
//PARAMETROS A SELECCIONAR POR EL USUARIO
cimg_usage("Compresión RLC con pérdidas");
const char* path_img = cimg_option("-i",(char*)0,"Ruta y nombre de la imagen que será cargada para comprimir.");
const char* path_comprimida = cimg_option("-o",(char*)0,"Ruta y nombre de la imagen comprimida que será guardada.");
const int AV = cimg_option("-a",7,"Ancho de la ventana.");
if (!path_img){
    cout<<"Especifique el nombre de la imagen a comprimir."<<endl;
    exit(1);
}
if (!path_comprimida){
    cout<<"Especifique el nombre de la imagen de salida."<<endl;
    exit(1);
}

//LEE LA IMAGEN
CImg<unsigned char> img(path_img);
unsigned short N=img.width();
unsigned short M=img.height();
if ((img.width()>65536)||(img.height()>65536)){
	cout<<"El tamaño máximo de la imagen debe ser de 65536x65536."<<endl;
	exit(1);
}

vector<unsigned char> I_comp;

//EXTRAE LOS BYTES ALTO Y BAJO DE LA CANTIDAD DE FILAS Y LOS GUARDA EN LOS
//ELEMENTOS 1 Y 2 DEL VECTOR I_COMP
unsigned char low = (M & 0xFFFFFFFF);
unsigned char high= ((M >> 8) & 11111111);
I_comp.push_back(high);
I_comp.push_back(low);

//EXTRAE LOS BYTES ALTO Y BAJO DE LA CANTIDAD DE COLUMNAS Y LOS GUARDA EN LOS
//ELEMENTOS 3 Y 4 DEL VECTOR I_COMP
low = (N & 0xFFFFFFFF);
high= ((N >> 8) & 11111111);
I_comp.push_back(high);
I_comp.push_back(low);

//PROCESO DE COMPRESION
vector<unsigned char> cad_cod;
for (int fil=0;fil<M;fil++){
    unsigned char R=img(0,fil);
    cad_cod.push_back(R);
    int min_v=R-(AV-1);
    int max_v=R+(AV-1);
    for (int col=1;col<N;col++){
        R=img(col,fil);
        if ((!(R<min_v))&&(!(R>max_v))&&(col<=N)){
            //col++;
            cad_cod.push_back(R);
            int min_v_nuevo=R-(AV-1);
            int max_v_nuevo=R+(AV-1);
            if (max_v_nuevo<max_v)
                max_v=max_v_nuevo;
            if (min_v_nuevo>min_v)
                min_v=min_v_nuevo;
        }
        else{
            int L_v=cad_cod.size();
            double acum=0;
            for (int i=0;i<L_v;i++)
                acum+=cad_cod[i];
            int mean=int(floor(acum/L_v));
            I_comp.push_back(L_v);
            I_comp.push_back(mean);
            cad_cod.clear();
            R=img(col,fil);
            cad_cod.push_back(R);
            min_v=R-(AV-1);
            max_v=R+(AV-1);
        }
    }
    int L_v=cad_cod.size();
    double acum=0;
    for (int i=0;i<L_v;i++)
        acum+=cad_cod[i];
    int mean=int(floor(acum/L_v));
    I_comp.push_back(L_v);
    I_comp.push_back(mean);
    cad_cod.clear();
}
cout<<"Verificando compresion..."<<endl;
double suma=0;
int i=4;
while (i<I_comp.size()){
    suma+=I_comp[i];
    i=i+2;
}
if (suma!=M*N){
    cout<<"Error al comprimir!!!"<<endl;
    exit(1);
}
else{
    ofstream img_comp(path_comprimida, ios::binary);
    if(img_comp.fail()){
        cerr << "Error al intentar guardar la imagen"<< endl;
        img_comp.close();
        exit(1);
    }
    else{
        for (int i=0;i<I_comp.size();i++){
            img_comp.write((char *) &I_comp[i],sizeof(unsigned char));
            if(img_comp.fail()){
                cerr<<"Error al intentar escribir la imagen"<< endl;
                img_comp.close();
                exit(1);
            }
        }
        cout<<"Exito al comprimir!!! Imagen comprimida guardada."<<endl;
        img_comp.close();
    }
}

//PROCESO DE DESCOMPRESION
vector<unsigned char> I_recons;
ifstream img_recons(path_comprimida, ios::binary);
//LECTURA DE LA IMAGEN COMPRIMIDA Y CARGA EN EL VECTOR I_RECONS
if(img_recons.fail()){
    cerr << "Error al intentar abrir la imagen comprimida"<< endl;
    img_recons.close();
    exit(1);
}
else{
    while (!img_recons.eof()){
        unsigned char aux;
        img_recons.read((char *) &aux, sizeof(unsigned char));
        if (!img_recons.eof()){
            if(img_recons.fail()){
                cerr<<"Error al intentar leer la imagen comprimida"<< endl;
                img_recons.close();
                exit(1);
            }
            I_recons.push_back(aux);
        }
    }
}
img_recons.close();

unsigned short word1=I_recons[0];
unsigned short word2=I_recons[1];
unsigned short M_recons=((word1<<8)|word2);
//cout<<"N�mero de filas: "<<M_recons<<endl;
word1=I_recons[2];
word2=I_recons[3];
unsigned short N_recons=((word1<<8)|word2);
//cout<<"N�mero de columnas: "<<N_recons<<endl;
CImg<unsigned char> imagen_recons(N_recons,M_recons,1,1,0);
int j=3; //posicion en la que empiezan los valores de intensidad en el vector I_recons
int columna=0;
int seed=time(0);
srand(seed);
for (int i=0;i<M_recons;i++){
    while (columna<N_recons){
       j++;
       int L_v=int(I_recons[j]); 
       j++;
       int mean=int(I_recons[j]);
       int min_v=mean-int(L_v/2);
       if (min_v<0)
           min_v=0;
       int max_v=mean+int(L_v/2);
       if (max_v>255)
           max_v=255;
       for (int k=0;k<L_v;k++){
           columna++;
           double r =(double)rand()/((double)(RAND_MAX)+(double)(1));
           unsigned char auxiliar=int(r*(max_v-min_v)+min_v);
           //cout<<"["<<min_v<<", "<<max_v<<"]->"<<int(auxiliar)<<endl;
           imagen_recons(columna,i)=auxiliar;
       }
    }
    columna=0;
}

CImgDisplay vent0(img,"Imagen original",0);
CImgDisplay vent1(imagen_recons,"Imagen reconstruida",0);

struct stat results;
double size1,size2;
if (stat(path_img, &results) == 0){
    size1=results.st_size/1024;
    cout<<"Tamaño de la imagen original: "<<size1<<" Kb."<<endl;
}
size2=I_comp.size()/1024;
cout<<"Tamaño de la imagen comprimida: "<<size2<<" Kb."<<endl;
cout<<"Tasa de compresión: "<< size1/size2<<endl;
cout<<"MSE orig-decomp:    "<<img.get_channel(0).MSE(imagen_recons)<<endl;
while (!vent0.is_closed());
exit(0);
}

