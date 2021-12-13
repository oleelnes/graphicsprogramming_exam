#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

struct BlockVertex {
    glm::vec3 location;
    glm::vec3 normals;
    glm::vec2 texCoords;
};

class Block {
public:
    Block();
    std::vector<BlockVertex> inactiveBlocks{};
    std::vector<BlockVertex> activeBlock{};
    unsigned int newActiveBlock(int type, float loc_x, float loc_y, float loc_z);
    void activeToInactive(int vertices, int x, int y, int z);
    unsigned int getInactiveBlocksVAO();
    unsigned int inactiveVAO;
   // unsigned int activeBlock(int type);
private:
    float cube_vertices[288]{
        //----position---- ------normal------ --texture-- 
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