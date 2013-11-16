#include "model.hpp"

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>


Model::Model() {
}

Model::Model(Model const &s) : indices(s.indices), vao(s.vao) {
}

Model::Model(std::string filename, GLuint positionAttribute, GLuint normalAttribute, GLuint uvAttribute) {
	loadModel(filename, positionAttribute, normalAttribute, uvAttribute);
}

Model::Model(std::string filename, ShaderProgram &s, std::string positionName, std::string normalName, std::string uvName) {
	loadModel(filename, s.getAttribLocation(positionName), s.getAttribLocation(normalName), s.getAttribLocation(uvName));
}

Model::Model(std::string filename, ShaderProgram &&s, std::string positionName, std::string normalName, std::string uvName) {
	loadModel(filename, s.getAttribLocation(positionName), s.getAttribLocation(normalName), s.getAttribLocation(uvName));
}

Model::Model(std::string filename, ShaderProgram &s) : Model(filename, s, "vertexPosition", "vertexNormal", "vertexUV") {
}

Model::Model(std::string filename, ShaderProgram &&s) : Model(filename, s, "vertexPosition", "vertexNormal", "vertexUV") {
}

void Model::loadModel(std::string filename, GLuint positionAttribute, GLuint normalAttribute, GLuint uvAttribute) {
	std::vector<unsigned int> temp_indices;
	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<VertexPTN> temp_vertices;

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
	    	std::vector<unsigned int> vertexIndex, uvIndex, normalIndex;
			std::stringstream s(line.substr(2));
			std::string line;

		    try {
		    	while(std::getline(s, line, ' ')) {
		    		std::string num = "";
		    		if(line.size() == 1)
		    			continue;
		    		int j = 0;
		    		for(unsigned int i = 0; i < line.size(); ++i) {
		    			if(line[i] == '/') {
		    				if(j == 0)
		    					vertexIndex.push_back(atoi(num.c_str()));
		    				else if(j == 1)
		    					uvIndex.push_back(atoi(num.c_str()));
		    				else if(j == 2)
		    					normalIndex.push_back(atoi(num.c_str()));
		    				num = "";
		    				j++;
		    			}
		    			else
		    				num += line[i];
		    		}
		    		if(j == 0)
	  					vertexIndex.push_back(atoi(num.c_str()));
	  				else if(j == 1)
	  					uvIndex.push_back(atoi(num.c_str()));
	  				else if(j == 2)
	  					normalIndex.push_back(atoi(num.c_str()));
		    	}
		  	}
		  	catch(...) {
		  		throw std::runtime_error("Error parsing model: " + filename);
		  	}

		  	if(vertexIndex.size() == 3) {
			    for(int i = 0; i < 3; ++i) {
			    	if(vertexIndex[i] - 1 >= temp_positions.size())
			    		throw std::runtime_error("Vertex index out of bounds in model " + filename);
			    	if(uvIndex[i] - 1 >= temp_uvs.size())
			    		throw std::runtime_error("UV index out of bounds in model " + filename);
			    	if(normalIndex[i] - 1 >= temp_normals.size())
			    		throw std::runtime_error("Normal index out of bounds in model " + filename);

			    	VertexPTN vert(temp_positions[vertexIndex[i] - 1], temp_uvs[uvIndex[i] - 1], temp_normals[normalIndex[i] - 1]);
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
		  	else if(vertexIndex.size() == 4) {
		  		int verts[6] = {0, 1, 3, 1, 2, 3};
		  		std::vector<unsigned int> vertexIndex3, uvIndex3, normalIndex3;
		  		for(int i = 0; i < 6; ++i) {
		  			vertexIndex3.push_back(vertexIndex[verts[i]]);
		  			uvIndex3.push_back(uvIndex[verts[i]]);
		  			normalIndex3.push_back(normalIndex[verts[i]]);
		  		}
		  		for(int i = 0; i < 6; ++i) {
			    	if(vertexIndex3[i] - 1 >= temp_positions.size())
			    		throw std::runtime_error("Vertex index out of bounds in model " + filename);
			    	if(uvIndex3[i] - 1 >= temp_uvs.size())
			    		throw std::runtime_error("UV index out of bounds in model " + filename);
			    	if(normalIndex3[i] - 1 >= temp_normals.size())
			    		throw std::runtime_error("Normal index out of bounds in model " + filename);

			    	VertexPTN vert(temp_positions[vertexIndex3[i] - 1], temp_uvs[uvIndex3[i] - 1], temp_normals[normalIndex3[i] - 1]);
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
	}

	temp_positions.clear();
	temp_uvs.clear();
	temp_normals.clear();
	for(auto i = temp_vertices.begin(); i != temp_vertices.end(); ++i) {
		temp_positions.push_back(i->position);
		temp_uvs.push_back(i->texture);
		temp_normals.push_back(i->normal);
	}

	Buffer positions;
	Buffer normals;
	Buffer uvs;

	positions.loadData(temp_positions);
	normals.loadData(temp_normals);
	uvs.loadData(temp_uvs);
	this->indices.loadData(temp_indices);

	this->vao.setAttrib(positionAttribute, positions, 3, GL_FLOAT);
	this->vao.setAttrib(normalAttribute, normals, 3, GL_FLOAT);
	this->vao.setAttrib(uvAttribute, uvs, 2, GL_FLOAT, true);
}

void Model::draw() {
	this->vao.bind();
	this->indices.drawElements();
}