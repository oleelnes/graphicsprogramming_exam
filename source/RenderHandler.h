#include "Shader.h"
#include "Cube.h"
#include "Camera.h"
#include <iostream>


class RenderHandler {
private:
	int gamemode1_state = 0;
	Cube* cube;

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
public:
	RenderHandler();
	int render(int gamemode);
	void renderer();
	void keyInput(int key, float time);
	void mouseInput(glm::vec3 direction);
	void init();
	void projectionInit();
	void cameraInit();
	void light();
	void transformations();
	//Shader* main_shader = new Shader("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");
};