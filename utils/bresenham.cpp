#include <vector>
#include <cmath>

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
	//delta_y > delta_x ?
	bool step = std::abs(y1-y0) > std::abs(x1-x0);

	unsigned int temp;
	if (step) {
		temp = y0;
		y0 = x0;
		x0 = temp;

		temp = y1;
		y1 = x1;
		x1 = temp;
	}

	if (x0 > x1) {
		temp = x0;
		x0 = x1;
		x1 = temp;

		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	int deltax = x1 - x0;
	int deltay = y1 - y0;

	int error = deltax / 2;

	int ystep;
	int y=y0;

	if (y0 < y1)
		ystep = 1;
	else
		ystep = -1;
    
	for (unsigned int x=x0; x < x1; x++) {
		std::vector<unsigned int> coord;
		if (step) {
			coord.push_back(y);
			coord.push_back(x);
			coords.push_back(coord);
		} else {
			coord.push_back(x);
			coord.push_back(y);
			coords.push_back(coord);
		}
		error = error - deltay;
		if (error < 0) {
			y = y + ystep;
			error = error + deltax;
		}

	}
}

} //End namespace