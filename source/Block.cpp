#include "Block.h"



Block::Block(){
//nothing yet
	for (int i = 0; i < 36 * 4; i++) {
		activeBlock.push_back({});
		auto& vertex = activeBlock.back();
		vertex.location = { 0.0f, 0.0f, 0.0f };
		vertex.normals = { 0.0f, 0.0f, 0.0f };
		vertex.texCoords = { 0.0f, 0.0f };
	}
}

unsigned int Block::newActiveBlock(int type, float loc_x, float loc_y, float loc_z, int roll, int pitch, int yaw){
	activeBlock.empty();
	int blocks = 1;

	float x_0 = 2.0f;
	float y_0 = 1.0f;

	float x_1 = 0.0f;
	float y_1 = 0.0f;

	float x_2 = 0.0f;
	float y_2 = 0.0f;

	float x_3 = 0.0f;
	float y_3 = 0.0f;

	switch (type) {
	case 0: //non-complex
		blocks = 1;
		break;
	case 1: //L block
		blocks = 4;
		x_1 = x_0 + 1.0f;
		y_1 = y_0 - 1.0f;
		x_2 = x_0;
		y_2 = y_0 + 1.0f;
		x_3 = x_0;
		y_3 = y_0 - 1.0f;
		break;
	case 2: //Z block
		blocks = 4;
		x_1 = x_0 - 1.0f;
		y_1 = y_0;
		x_2 = x_0;
		y_2 = y_0 + 1.0f;
		x_3 = x_0 + 1.0f;
		y_3 = y_0 + 1.0f;
		break;
	case 3: //T block
		blocks = 4;
		x_1 = x_0 + 1.0f;
		y_1 = y_0;
		x_2 = x_0 - 1.0f;
		y_2 = y_0;
		x_3 = x_0;
		y_3 = y_0 + 1.0f;
		break;
	default:
		break;
	}
	float curr_x = x_0;
	float curr_y = y_0;
	for (int b = 0; b < blocks; b++) {
		if (b == 0) { curr_x = x_0; curr_y = y_0; }
		else if (b == 1) { curr_x = x_1; curr_y = y_1; }
		else if (b == 2) { curr_x = x_2; curr_y = y_2; }
		else if (b == 3) { curr_x = x_3; curr_y = y_3; }

		if (roll > 0) {
			for (int i = 0; i < roll; i++) {
				float x_temp = curr_x;
				curr_x = -curr_y;
				curr_y = x_temp;
			}
			if (roll == 1) {
				curr_x += 3;
				curr_y -= 1;
			}
			if (roll == 2) {
				curr_x += 4;
				curr_y += 2;
			}
			if (roll == 3) {
				curr_x += 1;
				curr_y += 3;
			}
		}

		for (int v = 0; v < 36; v++) {
			//activeBlock.push_back({});
			//auto& vertex = activeBlock.back();

			activeBlock[v + 36 * b].location = { cube_vertices[(0 + v * 8)] + curr_x, cube_vertices[(1 + v * 8)] + curr_y, cube_vertices[(2 + v * 8)] + 0.0f};
			activeBlock[v + 36 * b].normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
			activeBlock[v + 36 * b].texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
		}
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

unsigned int Block::activeToInactive(int type, int x, int y, int z)
{
	//glDeleteVertexArrays(1, &inactiveVAO);
	/*
	int blocks = 1;

	float x_0 = 0.0f;
	float y_0 = 0.0f;

	float x_1 = 0.0f;
	float y_1 = 0.0f;

	float x_2 = 0.0f;
	float y_2 = 0.0f;

	float x_3 = 0.0f;
	float y_3 = 0.0f;

	switch (type) {
	case 0: //non-complex
		blocks = 1;
		break;
	case 1: //L block
		blocks = 4;
		x_1 = x_0 + 1.0f;
		y_1 = y_0 - 1.0f;
		x_2 = x_0;
		y_2 = y_0 + 1.0f;
		x_3 = x_0;
		y_3 = y_0 - 1.0f;
		break;
	case 2: //Z block
		blocks = 4;
		x_1 = x_0 - 1.0f;
		y_1 = y_0;
		x_2 = x_0;
		y_2 = y_0 + 1.0f;
		x_3 = x_0 + 1.0f;
		y_3 = y_0 + 1.0f;
		break;
	case 3: //T block
		blocks = 4;
		x_1 = x_0 + 1.0f;
		y_1 = y_0;
		x_2 = x_0 - 1.0f;
		y_2 = y_0;
		x_3 = x_0;
		y_3 = y_0 + 1.0f;
		break;
	default:
		break;
	}
	float curr_x = x_0;
	float curr_y = y_0;
	for (int b = 0; b < blocks; b++) {
		if (b == 0) { curr_x = x_0; curr_y = y_0; }
		else if (b == 1) { curr_x = x_1; curr_y = y_1; }
		else if (b == 2) { curr_x = x_2; curr_y = y_2; }
		else if (b == 3) { curr_x = x_3; curr_y = y_3; }
		for (int v = 0; v < 36; v++) {
			//activeBlock.push_back({});
			//auto& vertex = activeBlock.back();

			inactiveBlocks.push_back({});
			auto& vertex = inactiveBlocks.back();

			vertex.location = { activeBlock[v].location.x + curr_x + x, activeBlock[v].location.y + curr_y + y, activeBlock[v].location.z + z };
			vertex.normals = activeBlock[v].normals;
			vertex.texCoords = activeBlock[v].texCoords;


			//activeBlock[v + 36 * b].location = { cube_vertices[(0 + v * 8)] + curr_x, cube_vertices[(1 + v * 8)] + curr_y, cube_vertices[(2 + v * 8)] + 0.0f };
			//activeBlock[v + 36 * b].normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
			//activeBlock[v + 36 * b].texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
		}
	}*/

	/*
	for (int v = 0; v < 36; v++) {
		inactiveBlocks.push_back({});
		auto& vertex = inactiveBlocks.back();

		vertex.location = { activeBlock[v].location.x + x, activeBlock[v].location.y + y, activeBlock[v].location.z + z };
		vertex.normals = activeBlock[v].normals;
		vertex.texCoords = activeBlock[v].texCoords;
		//std::cout << "inactive vertex " << v << " location: " << inactiveBlocks[v].location.x << std::endl;
	}*/

	int multiplier = 1;
	if (type != 0) multiplier = 4;
	for (int i = 0; i < 36 * multiplier; i++) {
		//inactiveBlocks.push_back({});
		//auto& vertex = inactiveBlocks.back();

		inactiveBlocks.push_back({});
		auto& vertex = inactiveBlocks.back();

		vertex.location = { activeBlock[i].location.x  + x, activeBlock[i].location.y  + y, activeBlock[i].location.z + z };
		vertex.normals = activeBlock[i].normals;
		vertex.texCoords = activeBlock[i].texCoords;
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




