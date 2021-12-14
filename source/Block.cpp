#include "Block.h"



Block::Block(){
//nothing yet
}

unsigned int Block::newActiveBlock(int type, float loc_x, float loc_y, float loc_z){
	activeBlock.empty();
	for (int v = 0; v < 36; v++) {
		activeBlock.push_back({});
		auto& vertex = activeBlock.back();

		vertex.location = { cube_vertices[(0 + v * 8)] + 2.0f, cube_vertices[(1 + v * 8)] + 1.0f, cube_vertices[(2 + v * 8)] + 0.0f};
		vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
		vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
	}
	unsigned int activeBlockVAO;
	glGenVertexArrays(1, &activeBlockVAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(activeBlockVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * activeBlock.size(), activeBlock.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);

	return activeBlockVAO;
}

unsigned int Block::activeToInactive(int vertices, int x, int y, int z)
{
	//glDeleteVertexArrays(1, &inactiveVAO);
	
	for (int v = 0; v < vertices; v++) {
		inactiveBlocks.push_back({});
		auto& vertex = inactiveBlocks.back();

		vertex.location = { activeBlock[v].location.x + x, activeBlock[v].location.y + y, activeBlock[v].location.z + z };
		vertex.normals = activeBlock[v].normals;
		vertex.texCoords = activeBlock[v].texCoords;
		//std::cout << "inactive vertex " << v << " location: " << inactiveBlocks[v].location.x << std::endl;
	}
	unsigned int solidBlockVAO;
	glGenVertexArrays(1, &solidBlockVAO);
	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(solidBlockVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * inactiveBlocks.size(), inactiveBlocks.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);

	return solidBlockVAO;
}




