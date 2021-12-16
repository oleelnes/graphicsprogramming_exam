
#include "Shader.h"
#include "stb_image.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

class Tunnel {
public:
    glm::vec3 pointLightPositions[2] = {
        glm::vec3(2.5f,  2.5f,  5.0f),
        glm::vec3(0.7f,  0.2f,  2.0f)
    };
    Shader* shader = new Shader("resources/shaders/blockout.vs", "resources/shaders/blockout.fs");
    unsigned int createCubeTunnel(int x_stride, int y_stride, int z_stride);
private:
	float cube_vertices[288]{
     //----position----    ------normal------  -- texture--
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //back
    1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //front
    1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //left
    0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //right
    1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,//bottom
    1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //top
    1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,

	};

};