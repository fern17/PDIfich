// ================== Description  ==========================
//
// To use the function:
// huffmanCodification(data[], HuffmanCodes[], Entropia, LongProm, Compresion, dataSize);
// huffmanCodification(data[], HuffmanCodes[], Compresion, dataSize);
// huffmanCodification(data[], HuffmanCodes[], dataSize);
//
// IN:
// data[] : array of double with simbol probabilities or occurrences.
//          It could be an image histogram.
//
// OUT:
// HuffmanCodes[] : array of string, return huffman codification
// Entropia : return the Entropy (for huffman codes)
// LongProm : return the average word length with huffman codes
// LongProm : return Compression rate
// dataSize : data[] size
//
//
// Enrique Marcelo Albornoz
// Date: 2009-05-07
// Catedra de Procesamiento Digital de Imágenes
// Centro de I+D en Señales, Sistemas e Inteligencia Computacional (SINC),
// Dpto. Informática, Facultad de Ingeniería y Cs Hídricas,
// Universidad Nacional del Litoral, Santa Fe (Argentina)
// web: http://fich.unl.edu.ar/sinc/
//
// ==========================================================
#include <stdlib.h>
#include <valarray>
#include <list>
#include <string>
#include <CImg.h>

using namespace std;
using namespace cimg_library;

typedef list<bool> ListaDeBool;
struct arbol{
    double data;
    int indice;
    ListaDeBool codes;
    struct arbol *left;
    struct arbol *right;
};
typedef struct arbol node;
typedef node *btree;


void ordenar(list<node*>& probs){
    int auxindx, ord_i_ind, ord_j_ind;
    list<node*>::iterator ord_i, ord_j, ord_aux;
    double aux1, aux2;
    ord_i = probs.end();
    for(ord_i_ind = probs.size();ord_i_ind > 1; ord_i_ind--){
        ord_j = probs.begin();
        for(int ord_j_ind = 1; ord_j_ind < ord_i_ind; ord_j_ind++){
            aux1 = (double)((*ord_j)->data);
            ord_aux = ord_j;
            advance(ord_aux,1);
            aux2=(double)((*ord_aux)->data);
            if (aux1 > aux2){
                probs.insert(ord_j, (*ord_aux));
                probs.erase(ord_aux);
            }else{
                ord_j++;
            }
        }
        ord_i--;
    }
}

void huffman(list<node*>& probs) {
    list<node*>::iterator c_c_i,c_c_j;
    valarray<int> indices (probs.size()+1);

    while ((int)probs.size() > 2) {
        ordenar(probs);
        c_c_i = probs.begin();
        c_c_j = probs.begin();
        advance(c_c_i,1);
        node *a=new node;
        a->data = ((*c_c_j)->data) + ((*c_c_i)->data);
        a->indice = 0;
        a->left = *c_c_j;
        a->right = *c_c_i;
        probs.erase(probs.begin());
        probs.erase(probs.begin());
        probs.push_front(a);
    }
}

void CodesSet(btree& root){

    list<bool>::iterator codes_i, codes_j;

    if(root->left != NULL){
        for (codes_i = (root->codes).begin(); codes_i != (root->codes).end() ; codes_i++){
            (root->left->codes).push_back(*codes_i);
            (root->right->codes).push_back(*codes_i);
        }

		double www1, www2;
		www1=(float)((root->left)->data);
		www2=(float)((root->right)->data);
	
		if (www1 == www2){
			if(root->left->left != NULL){
				(root->left->codes).push_back(1);
				(root->right->codes).push_back(0);
			}else{
				(root->left->codes).push_back(0);
				(root->right->codes).push_back(1);
			}
		} else {
			if (www1 > www2){
				(root->left->codes).push_back(0);
				(root->right->codes).push_back(1);
			}else{
				(root->left->codes).push_back(1);
				(root->right->codes).push_back(0);
			}
		}
	
		CodesSet(root->left);
		CodesSet(root->right);
    }
}

void setcodes(list<node*>& AH) {
    list<node*>::iterator i_A1,i_A2;
    i_A1 = AH.begin();
	i_A2 = i_A1;
    advance(i_A2, 1);
	
	if ((*i_A1)->data > (*i_A2)->data){
    	(*i_A1)->codes.push_back(0);
    	CodesSet(*i_A1);
    	(*i_A2)->codes.push_back(1);
    	CodesSet(*i_A2);
	}else{
    	(*i_A1)->codes.push_back(1);
    	CodesSet(*i_A1);
    	(*i_A2)->codes.push_back(0);
    	CodesSet(*i_A2);
	}
}


void CodesSearch(btree root, valarray<ListaDeBool>& S1){
    if(root != NULL){
        CodesSearch(root->left, S1);
        if ((root->indice) != 0){
            S1[root->indice] = root->codes;
        }
        CodesSearch(root->right, S1);
    }
}

void getcodes(list<node*> AH, valarray<ListaDeBool>& S) {
    list<node*>::iterator i_A;
    i_A = AH.begin();
    CodesSearch(*i_A, S);
    advance(i_A, 1);
    CodesSearch(*i_A, S);
}

// // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // //
void huffmanCodification(double data[], string HuffmanCodes[], double& Entropia, double& LongProm, double& Compresion, int dataSize){

    if (dataSize > 2){

        list<node*> ListaDePTR, ARBOLHUFFMAN;
        list<node*>::iterator Lista_it;
        valarray<ListaDeBool> Simbolos(dataSize + 1);
        valarray<double> arr(dataSize + 1);
    
        arr[0] = 0;
        for (int n=1; n < arr.size(); n++){
            arr[n] = data[n-1];
        }
    
        double Sumatoria = arr.sum();
        for (int n=1; n < arr.size(); n++){
            arr[n] = arr[n]/Sumatoria;
            if (arr[n] > 0){
                node *a = new node;
                a->data = arr[n];
                a->indice = n;
                a->left = NULL;
                a->right = NULL;
                ListaDePTR.push_back(a);
            }
        }
    
        huffman(ListaDePTR);
        setcodes(ListaDePTR);
        getcodes(ListaDePTR, Simbolos);
    

        LongProm = 0;
        Entropia = 0;
        for (int n=1; n < Simbolos.size(); n++){
            if (arr[n] > 0){
                LongProm = LongProm + (arr[n] * Simbolos[n].size());
                Entropia = Entropia - (arr[n] * log2(arr[n]));
                for (list<bool>::iterator it_b = Simbolos[n].begin(); it_b != Simbolos[n].end(); it_b++){
                    if (*it_b == 1){
                        HuffmanCodes[n-1] += "1";
                    }else{
                        HuffmanCodes[n-1] += "0";
                    }
                }
            }
        }
        Compresion = (log(dataSize)/log(2)) / LongProm;
    }
}

void huffmanCodification(double data[], string HuffmanCodes[], int dataSize){
    double E, L, C;
    huffmanCodification(data, HuffmanCodes, E, L, C, dataSize);
}

void huffmanCodification(double data[], string HuffmanCodes[], double& Compresion, int dataSize){
    double E, L, C;
    huffmanCodification(data, HuffmanCodes, E, L, C, dataSize);
}

void print_huffman(CImg<> probabilities){
  // probabilities: Image array with simbol probabilities or occurrences.
  //                This array could be the image histogram.
  
  double AvgWord, Entropy, CompRate;
  double SumAll=0;
  int dSize = probabilities.width();
  double data[dSize];
  string HuffmanInfo[dSize];
  
  for (int n=0; n < dSize; n++){
    data[n] = probabilities(n);
    SumAll= SumAll + data[n];
  }

  huffmanCodification(data, HuffmanInfo, Entropy, AvgWord, CompRate, dSize);

  // // // // // // // // // // // // // // // // // // // // // // // // // // // //
  // // //                     SHOW RESULTS
  // // // // // // // // // // // // // // // // // // // // // // // // // // // //
  for (int n=0; n < dSize; n++){
    cout << n << ":     initial value: " << data[n];
    cout << "     probability: " << (data[n]/SumAll);
    if (data[n] > 0){
      cout  << "     Huffman code: " << HuffmanInfo[n];
    }
    cout << endl;
  }
  cout << endl << "Entropy:             " << Entropy;
  cout << endl << "Average word length: " << AvgWord;
  cout << endl << "Compression Rate:    " << CompRate << endl << endl;
  // // // // // // // // // // // // // // // // // // // // // // // // // // // //
  // // // // // // // // // // // // // // // // // // // // // // // // // // // //

}
