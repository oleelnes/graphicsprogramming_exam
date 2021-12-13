#include "Cube.h"

struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

Cube::Cube(){
	//glGenVertexArrays(1, &cubeVAO);
	//createCubeVAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gridTexture = load_opengl_texture("resources/textures/grid_tile.png");
	activeBlockTexture = load_opengl_texture("resources/textures/active_block_tile.png");
}

void Cube::createCubeVAO(){
	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(this->cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	//Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

unsigned int Cube::createCustomCube(float length, float height, float depth, float heightOffset){
	float tempArray[288];
	for (int i = 0; i < 288; i++) {
		if (i % 8 == 0) {
			tempArray[i] = cube_vertices[i] * length;
		}
		else if (i % 8 == 1) {
			tempArray[i] = (cube_vertices[i] * height) + heightOffset;
		}
		else if (i % 8 == 2) {
			tempArray[i] = cube_vertices[i] * depth;
		}
		//TODO: try putting else if on the two above and simply else on this one instead.
		else{
			tempArray[i] = cube_vertices[i];
		}
	}
	unsigned int tempVAO;
	glGenVertexArrays(1, &tempVAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(tempVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tempArray), tempArray, GL_STATIC_DRAW);

	//Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return tempVAO;
}

unsigned int Cube::createCustomLightCube(float length, float height, float depth, float lengthOffset, float heightOffset, float depthOffset)
{
	float tempArray[288];
	for (int i = 0; i < 288; i++) {
		if (i % 8 == 0) {
			tempArray[i] = (cube_vertices[i] * length) + lengthOffset;
		}
		else if (i % 8 == 1) {
			tempArray[i] = (cube_vertices[i] * height) + heightOffset;
		}
		else if (i % 8 == 2) {
			tempArray[i] = (cube_vertices[i] * depth) + depthOffset;
		}
		else {
			tempArray[i] = cube_vertices[i];
		}
	}
	unsigned int tempVAO;
	glGenVertexArrays(1, &tempVAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(tempVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 288 , &tempArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	return tempVAO;
}

unsigned int Cube::createCubeTunnel(int x_stride, int y_stride, int z_stride)
{
	const int left_plane = (y_stride * z_stride);
	const int right_plane = left_plane;
	const int top_plane = (x_stride * z_stride);
	const int bottom_plane = top_plane;
	const int far_plane = (y_stride * y_stride);
	const int num_cubes = top_plane + bottom_plane + left_plane + right_plane + far_plane;

	std::vector<Vertex> tempVec{};
	for (int curr_plane = 0; curr_plane < 5; curr_plane++) {
		//left plane
		if (curr_plane == 0) {
			for (int y = 0; y < y_stride; y++) {
				for (int z = 0; z < z_stride; z++) {
					for (int v = 18; v < 24; v++) {
						tempVec.push_back({});
						auto& vertex = tempVec.back();

						vertex.location = { cube_vertices[(0 + v * 8)] - 1.0f, cube_vertices[(1 + v * 8)] + y, cube_vertices[(2 + v * 8)] + z};
						vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
						vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
					}
				}
			}
		}
		//right plane
		if (curr_plane == 1) {
			int x = x_stride;
			for (int y = 0; y < y_stride; y++) {
				for (int z = 0; z < z_stride; z++) {
					for (int v = 12; v < 18; v++) {
						tempVec.push_back({});
						auto& vertex = tempVec.back();

						vertex.location = { cube_vertices[(0 + v * 8)] + x, cube_vertices[(1 + v * 8)] + y, cube_vertices[(2 + v * 8)] + z };
						vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
						vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
					}
				}
			}
		}
		//bottom plane
		if (curr_plane == 2) {
			float y = -1.0f;
			for (int x = 0; x < x_stride; x++) {
				for (int z = 0; z < z_stride; z++) {
					for (int v = 30; v < 36; v++) {
						tempVec.push_back({});
						auto& vertex = tempVec.back();

						vertex.location = { cube_vertices[(0 + v * 8)] + x, cube_vertices[(1 + v * 8)] + y, cube_vertices[(2 + v * 8)] + z };
						vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
						vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
					}
				}
			}
		}
		//top plane
		if (curr_plane == 3) {
			int y = y_stride;
			for (int x = 0; x < x_stride; x++) {
				for (int z = 0; z < z_stride; z++) {
					for (int v = 24; v < 30; v++) {
						tempVec.push_back({});
						auto& vertex = tempVec.back();

						vertex.location = { cube_vertices[(0 + v * 8)] + x, cube_vertices[(1 + v * 8)] + y, cube_vertices[(2 + v * 8)] + z };
						vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
						vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
					}
				}
			}
		}
		//far plane
		if (curr_plane == 3) {
			int z = z_stride;
			for (int x = 0; x < x_stride; x++) {
				for (int y = 0; y < y_stride; y++) {
					for (int v = 0; v < 6; v++) {
						tempVec.push_back({});
						auto& vertex = tempVec.back();

						vertex.location = { cube_vertices[(0 + v * 8)] + x, cube_vertices[(1 + v * 8)] + y, cube_vertices[(2 + v * 8)] + z };
						vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
						vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
					}
				}
			}
		}
	}

	//std::cout << "entries: " << tempVec.size() << std::endl;

	std::cout << "tempvec location 3 x: " << tempVec[1].location.x << std::endl;


	unsigned int tunnel_VAO;
	glGenVertexArrays(1, &tunnel_VAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(tunnel_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * tempVec.size(), tempVec.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	//i < 5 -> 5 planes to be drawn
	//int cube_counter = 0;
	
	
	return tunnel_VAO;
}


GLuint Cube::load_opengl_texture(const std::string& filepath)
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