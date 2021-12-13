#include "Block.h"



unsigned int Block::addBlock(int type, float loc_x, float loc_y, float loc_z){
	blocks.empty();
	for (int v = 0; v < 36; v++) {
		blocks.push_back({});
		auto& vertex = blocks.back();

		vertex.location = { cube_vertices[(0 + v * 6)] + 2.0f, cube_vertices[(1 + v * 6)] + 1.0f, cube_vertices[(2 + v * 6)] + 0.0f};
		vertex.normals = { cube_vertices[3 + v * 6], cube_vertices[4 + v * 6], cube_vertices[5 + v * 6] };
	}
	for (int v = 0; v < 36; v++) {
		blocks.push_back({});
		auto& vertex = blocks.back();

		vertex.location = { cube_vertices[(0 + v * 6)] + 2.0f, cube_vertices[(1 + v * 6)] + 2.0f, cube_vertices[(2 + v * 6)] + 0.0f};
		vertex.normals = { cube_vertices[3 + v * 6], cube_vertices[4 + v * 6], cube_vertices[5 + v * 6] };
	}
	for (int v = 0; v < 36; v++) {
		blocks.push_back({});
		auto& vertex = blocks.back();

		vertex.location = { cube_vertices[(0 + v * 6)] + 2.0f, cube_vertices[(1 + v * 6)] + 3.0f, cube_vertices[(2 + v * 6)] + 0.0f };
		vertex.normals = { cube_vertices[3 + v * 6], cube_vertices[4 + v * 6], cube_vertices[5 + v * 6] };
	}
	for (int v = 0; v < 36; v++) {
		blocks.push_back({});
		auto& vertex = blocks.back();

		vertex.location = { cube_vertices[(0 + v * 6)] + 3.0f, cube_vertices[(1 + v * 6)] + 1.0f, cube_vertices[(2 + v * 6)] + 0.0f };
		vertex.normals = { cube_vertices[3 + v * 6], cube_vertices[4 + v * 6], cube_vertices[5 + v * 6] };
	}
	unsigned int blocksVAO;
	glGenVertexArrays(1, &blocksVAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(blocksVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * blocks.size(), blocks.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);

	return blocksVAO;
}

