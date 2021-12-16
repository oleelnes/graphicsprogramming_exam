#include "Tunnel.h"

/*
	This class contains all the necessary functions and variables to create the VAO of a tunnel
*/


struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};


/*
	This function creates the tunnel that the game takes place in
	int x_stride: The width of the tunnel
	int y_stride: The height of the tunnel
	int z_stride: The depth of the tunnel
*/
unsigned int Tunnel::createCubeTunnel(int x_stride, int y_stride, int z_stride)
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
	
	return tunnel_VAO;
}

