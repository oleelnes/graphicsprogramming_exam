
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
    Shader* shader = new Shader("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");
    Shader* lightCubeShader = new Shader("resources/shaders/light_cube.vs", "resources/shaders/light_cube.fs");
    unsigned int cubeVAO;
    Tunnel();
    void createCubeVAO();
    unsigned int createCubeTunnel(int x_stride, int y_stride, int z_stride);


private:
	float cube_vertices[288]{
     //----position----    ------normal------  -- texture--
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //bak
    1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //
    1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //venstre
    0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //høyre
    1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,//bunn
    1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //topp
    1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,

	};

};