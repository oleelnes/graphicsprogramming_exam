#include "TextureLoader.h"

TextureLoader::TextureLoader(){
	gridTexture = load_opengl_texture("resources/textures/grid_tile.png");
	activeBlockTexture = load_opengl_texture("resources/textures/active_block_tile.png");
	layer_1 = load_opengl_texture("resources/textures/layer_1.png");
	layer_2 = load_opengl_texture("resources/textures/layer_2_ubrown.png");
	layer_3 = load_opengl_texture("resources/textures/layer_3_babyblue.png");
	layer_4 = load_opengl_texture("resources/textures/layer_4_pink.png");
	layer_5 = load_opengl_texture("resources/textures/layer_5_dblue.png");
	layer_6 = load_opengl_texture("resources/textures/layer_6_orange.png");
	layer_7 = load_opengl_texture("resources/textures/layer_7_white.png");
	layer_8 = load_opengl_texture("resources/textures/layer_8_yellow.png");
	layer_9 = load_opengl_texture("resources/textures/layer_9_red.png");
	layer_10 = load_opengl_texture("resources/textures/layer_10_blue.png");

}

GLuint TextureLoader::load_opengl_texture(const std::string& filepath)
{
	int w, h, bpp;


	auto pixels = stbi_load(filepath.c_str(), &w, &h, &bpp, STBI_rgb_alpha);
	if (!pixels) std::cout << "error: image failed to load" << std::endl;
	else std::cout << "success! image loaded" << std::endl;
	GLuint tex;
	glGenTextures(1, &tex);
	//glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	if (pixels) stbi_image_free(pixels);

	return tex;
}
