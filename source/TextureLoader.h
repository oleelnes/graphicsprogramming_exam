#include "Shader.h"
#include "stb_image.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

class TextureLoader {
public:
	TextureLoader();

	GLuint gridTexture;
	GLuint activeBlockTexture;
	//GLuint inactiveTexture;
	GLuint layer_10;
	GLuint layer_9;
	GLuint layer_8;
	GLuint layer_7;
	GLuint layer_6;
	GLuint layer_5;
	GLuint layer_4;
	GLuint layer_3;
	GLuint layer_2;
	GLuint layer_1;


private:
	GLuint load_opengl_texture(const std::string& filepath);

};