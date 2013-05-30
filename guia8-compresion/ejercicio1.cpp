#include <CImg.h>               //include basico de CImg
#include <iostream>
#include <cmath>
#include <vector>
using namespace cimg_library;   //Necesario

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


int main(int argc, char *argv[]) {
	const char* input = cimg_option("-i", "../images/estanbul.tif", "Input Image File");

	CImg<double> imagen(input);
	imagen.get_quantize(8);

	CImg<double> histograma = imagen.get_histogram(8) / imagen.size();
	std::vector<unsigned char> vectorgris;

	
	for (unsigned int i=0; i < 8; i++){
		vectorgris.push_back(i);
	}
	BubbleSort(histograma, vectorgris);

	for (unsigned int i=0; i < 8; i++){
		std::cout<<(unsigned int) vectorgris[i]<<' '<<histograma(i,0)<<'\n';
	}


	histograma.display();
}