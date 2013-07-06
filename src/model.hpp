#ifndef MODEL_H
#define MODEL_H
#include <string>

#include "graphics.hpp"

class Model {
	public:
		Model(string filename);

		void draw(Graphics g, float x, float y, float scale);	
};

#endif