#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
namespace utils{

void get_bresenham_coords(  unsigned int H,
							unsigned int W, 
							unsigned int x0,
							unsigned int y0,
							unsigned int x1,
							unsigned int y1,
							std::vector<std::vector<unsigned int> > & coords
						)	{
	coords.clear();
    //y1 = H - y1;
    //y0 = H - y0;
    int dx = x1-x0,
        dy = y1-y0,
        y = y0,
        eps = 0;
    for (int x = x0; x <= x1; x++) {
        std::vector<unsigned int> coord;
        coord.push_back(x);
        coord.push_back(y);
        coords.push_back(coord);
        eps += dy;
        if ( (eps << 1) >= dx) {
            y++;
            eps -= dx;
        }
    }
    
    
  /* 
    int deltax = x1 - x0;
	assert(deltax != 0);
	int deltay = y1 - y0;
	float error = 0;
	float deltaerr = fabs( ((float) deltay) / ((float) deltax)  );
    std::cout<<"Deltaerr = "<<deltaerr<<'\n';
	int y = y0;
	for (int x = x0; x < x1; x++) {
		std::vector<unsigned int> coord;
		coord.push_back(x);
		coord.push_back(y);
		coords.push_back(coord);

		error = error + deltaerr;

		if (error >= 0.5) {
			y = y - 1; //negativo porque las coordenadas estan al reves
			error = error - 1.0;
		}
	}*/
// 	coords.clear();
// 	//delta_y > delta_x ?
// 	bool step = std::abs(y1-y0) > std::abs(x1-x0);

// 	unsigned int temp;
// 	if (step) {
// 		temp = y0;
// 		y0 = x0;
// 		x0 = temp;

// 		temp = y1;
// 		y1 = x1;
// 		x1 = temp;
// 	}

// 	if (x0 > x1) {
// 		temp = x0;
// 		x0 = x1;
// 		x1 = temp;

// 		temp = y0;
// 		y0 = y1;
// 		y1 = temp;
// 	}

// 	int deltax = x1 - x0;
// 	int deltay = y1 - y0;

// 	int error = deltax / 2;

// 	int ystep;
// 	int y=y0;

// 	if (y0 < y1)
// 		ystep = 1;
// 	else
// 		ystep = -1;
    
// 	for (unsigned int x=x0; x < x1; x++) {
// 		std::vector<unsigned int> coord;
// 		if (step) {
// 			coord.push_back(y);
// 			coord.push_back(x);
// 			coords.push_back(coord);
// 		} else {
// 			coord.push_back(x);
// 			coord.push_back(y);
// 			coords.push_back(coord);
// 		}
// 		error = error - deltay;
// 		if (error < 0) {
// 			y = y + ystep;
// 			error = error + deltax;
// 		}

// 	}
}

} //End namespace
