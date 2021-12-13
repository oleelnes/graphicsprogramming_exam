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
};

class Block {
public:
    std::vector<BlockVertex> blocks{};
    std::vector<BlockVertex> activeBlock{};
    unsigned int addBlock(int type, float loc_x, float loc_y, float loc_z);
   // unsigned int activeBlock(int type);
private:
    float cube_vertices[216]{
        //----position---- ------normal------  
       0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  //bak
       1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
       1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
       1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
       0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
       0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 

       0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  //
       1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 
       1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 
       1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 
       0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 
       0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 

       0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,  //venstre
       0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 
       0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 
       0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 
       0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, 
       0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 

       1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  //høyre
       1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 
       1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 
       1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 
       1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, 
       1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 

       0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, //bunn
       1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 
       1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 
       1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 
       0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 
       0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 

       0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  //topp
       1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 
       1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 
       1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 
       0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 
       0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 

    };
};