#include "Shader.h"
#include "Cube.h"
#include "Camera.h"
#include "Block.h"
#include <iostream>


class RenderHandler {
private:
	void renderActiveBlock();
	unsigned int blocksVAO;
	unsigned int tunnelVAO;
	bool firstActiveBlockCall; //creates the VAO for the block!
	int activeBlock_x;
	int activeBlock_y;
	int activeBlock_z;

	bool activeBlockIsMoving; //when true it makes it so that player cannot rotate or move it anymore!

	int gamemode1_state = 0;
	Cube* cube;
	Block* block;

	glm::mat4 model;
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

	Camera camera;
	void init();
	void projectionInit();
	void cameraInit();
	void light();
	void transformations();
public:
	RenderHandler();
	int render(int gamemode);
	void renderer();
	void keyInput(int key, float time);
	void mouseInput(glm::vec3 direction);
	//Shader* main_shader = new Shader("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");
};