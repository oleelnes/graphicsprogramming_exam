#include "Shader.h"
#include "TextureLoader.h"
#include "Cube.h"
#include "Block.h"
#include <iostream>
#include <cstdlib>

struct InactiveBlocksVertices {
	int type;
	int vertices;
	int layer;
};

class RenderHandler {
public:
	RenderHandler();
	int render(int gamemode);
	void renderer();
	void keyInput(int key, float time);
	void updateSpeed(float gamespeed);
	void updateTime(float time);
	void descend();
	int getScore();
	int getLayer();
	void mouseInput(glm::vec3 direction);
	//Shader* main_shader = new Shader("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");

private:
	void renderActiveBlock();
	unsigned int blocksVAO;
	unsigned int tunnelVAO;
	unsigned int inactiveVAO;
	bool firstActiveBlockCall; //creates the VAO for the block!
	bool inactive;

	int activeBlock_x;
	int activeBlock_y;
	float activeBlock_z;
	float speed;
	int solidBlocks;
	int grid[12 * 5 * 5];
	int gridZLoc;
	int score;
	int layer;
	int activeBlockType;
	int lastActiveBlockType;
	int activeBlocks;
	std::vector<InactiveBlocksVertices> inactiveBlockVertices;

	bool activeBlockIsMoving; //when true it makes it so that player cannot rotate or move it anymore!
	int drawcallcounter;

	int gamemode1_state = 0;
	Cube* cube;
	Block* block;
	TextureLoader* texture;

	glm::mat4 model;
	glm::mat4 activeModel;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 wallModel;
	glm::mat4 wallView;
	glm::mat4 wallProjection;
	int modelLoc, viewLoc, projectionLoc, wModelLoc, wViewLoc, wProjectionLoc;

	//camera
	glm::vec3 cameraPos;
	glm::vec3 lastCameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	bool staticActiveBlockCollision(int key);
	int getGridXLoc();
	int getGridYLoc();
	void renderInactiveBlock(int requestedLayer, GLuint& layer);
	void renderAllSolidBlocks();
	void moveActiveBlock();
	bool movingActiveBlockCollision();
	void newActiveBlock();
	void init();
	void projectionInit();
	void cameraInit();
	void light();
	void transformations();
	void setGridZLoc(int currGridLoc);
};