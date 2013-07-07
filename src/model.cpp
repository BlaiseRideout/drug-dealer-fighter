#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include "model.hpp"

Model::Model(std::string filename) {
	std::vector<unsigned int> temp_indices;
	std::vector<glm::vec3> 		temp_positions;
	std::vector<glm::vec2> 		temp_uvs;
	std::vector<glm::vec3> 		temp_normals;
	std::vector<Vertex> 			temp_vertices;

	std::ifstream file(filename, std::ios::in);
	if(!file.is_open())
		throw std::runtime_error("Cannot load model: " + filename);

	std::string line = "";

	while(std::getline(file, line)){
    if(line.substr(0, 2) == "v "){
	    glm::vec3 vertex;
	    std::stringstream s(line.substr(2));
	    s >> vertex.x;
	    s >> vertex.y;
	    s >> vertex.z;
	    temp_positions.push_back(vertex);
		}
		else if(line.substr(0, 2) == "vt"){
	    glm::vec2 uv;
	    std::stringstream s(line.substr(3));
	    s >> uv.x;
	    s >> uv.y;
	    temp_uvs.push_back(uv);
		}
		else if(line.substr(0, 2) == "vn"){
			std::stringstream s(line.substr(3));
	    glm::vec3 normal;
	    s >> normal.x;
	    s >> normal.y;
	    s >> normal.z;
	    temp_normals.push_back(normal);
		}
		else if(line[0] == 'f'){
			std::stringstream s(line.substr(2));
	    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	    std::string num;

	    try {
		    std::getline(s, num, '/');
				vertexIndex[0] = atof(num.c_str());
		    std::getline(s, num, '/');
		    uvIndex[0] = atof(num.c_str());
		    std::getline(s, num, ' ');
		    normalIndex[0] = atof(num.c_str());

		    std::getline(s, num, '/');
		    vertexIndex[1] = atof(num.c_str());
		    std::getline(s, num, '/');
		    uvIndex[1] = atof(num.c_str());
		    std::getline(s, num, ' ');
		    normalIndex[1] = atof(num.c_str());

		    std::getline(s, num, '/');
		    vertexIndex[2] = atof(num.c_str());
		    std::getline(s, num, '/');
		    uvIndex[2] = atof(num.c_str());
		    std::getline(s, num);
		    normalIndex[2] = atof(num.c_str());
	  	}
	  	catch(...) {
	  		throw std::runtime_error("Error parsing model: " + filename);
	  	}

	    for(int i = 0; i < 3; ++i) {
	    	if(vertexIndex[i] - 1 >= temp_positions.size())
	    		throw std::runtime_error("Vertex index out of bounds in model " + filename);
	    	if(uvIndex[i] - 1 >= temp_uvs.size())
	    		throw std::runtime_error("UV index out of bounds in model " + filename);
	    	if(normalIndex[i] - 1 >= temp_normals.size())
	    		throw std::runtime_error("Normal index out of bounds in model " + filename);

	    	Vertex vert(temp_positions[vertexIndex[i] - 1], temp_uvs[uvIndex[i] - 1], temp_normals[normalIndex[i] - 1]);
	    	unsigned int j;
	    	for(j = 0; j < temp_vertices.size() && temp_vertices[j] != vert; ++j);

		    if(j == temp_vertices.size()) {
		    	temp_indices.push_back(temp_vertices.size());
		    	temp_vertices.push_back(vert);
		    }
		    else
		    	temp_indices.push_back(j);
	  	}
		}
	}

	temp_positions.clear();
	temp_uvs.clear();
	temp_normals.clear();
	for(auto i = temp_vertices.begin(); i != temp_vertices.end(); ++i) {
		temp_positions.push_back(i->position);
		temp_uvs.push_back(i->texture);
		temp_normals.push_back(i->normal);
	}

	glGenBuffers(1, &this->positions);
	glBindBuffer(GL_ARRAY_BUFFER, this->positions);
	glBufferData(GL_ARRAY_BUFFER, temp_positions.size() * sizeof(glm::vec3), &temp_positions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->uvs);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvs);
	glBufferData(GL_ARRAY_BUFFER, temp_uvs.size() * sizeof(glm::vec2), &temp_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->normals);
	glBufferData(GL_ARRAY_BUFFER, temp_normals.size() * sizeof(glm::vec3), &temp_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, temp_indices.size() * sizeof(unsigned int), &temp_indices[0], GL_STATIC_DRAW);
	this->numIndices = temp_indices.size();
}

void Model::draw() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->positions);
	glVertexAttribPointer(
     0,                  // attribute 0
     3,                  // size
     GL_FLOAT,           // type
     GL_FALSE,           // normalized?
     0,                  // stride
     (void*)0            // array buffer offset
  );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvs);
	glVertexAttribPointer(
      1,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
  );

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normals);
	glVertexAttribPointer(
      2,                                // attribute
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
  );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices);
	glDrawElements(
	   GL_TRIANGLES,      // mode
	   this->numIndices,  // count
	   GL_UNSIGNED_INT,   // type
	   (void*)0           // element array buffer offset
	);
}