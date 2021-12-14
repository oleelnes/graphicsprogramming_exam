#include "RenderHandler.h"

/*
	This class creates pointers to:
		* Shader (main_shader)
		* Overlay (on-screen and real-time informative text such as score, health, stamina.
	The Shader-pointer is sent to the functions in the draw-functions in the gamemode-classes.
*/



RenderHandler::RenderHandler(){
	cube = new Cube();
	block = new Block();
	cameraInit();
	projectionInit();
	//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //initiating the camera. 
	firstActiveBlockCall = true;
	activeBlockIsMoving = false;
	inactive = false;
	glGenVertexArrays(1, &tunnelVAO);
	tunnelVAO = cube->createCubeTunnel(5, 5, 10);
	activeBlock_x = 0;
	activeBlock_y = 0;
	activeBlock_z = 0;
	for (int i = 0; i < (11 * 5 * 5) + 25; i++) {
		if (i <= 250)
			grid[i] = 0;
		else
			grid[i] = 1;
	}
	inactiveBlockVertices.push_back({});
	auto& vertex = inactiveBlockVertices.back();
	//vertex.index = 1;
	vertex.vertices = 0;
}

void RenderHandler::renderer(){
	

	cube->shader->use();
	light();
	transformations();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//creating tunnel
	glBindVertexArray(tunnelVAO);
	cube->shader->setInt("withTexture", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube->gridTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36*(((5*10)*4)+(5*5)));
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//cube->shader->setInt("withTexture", 0);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	if (activeBlockIsMoving) moveActiveBlock();
	renderActiveBlock();
	if (inactive) renderInactiveBlock();

	glBindVertexArray(0);

	//cube->lightCubeShader->use();
	//cube->lightCubeShader->setMat4("projection", projection);
	//cube->lightCubeShader->setMat4("view", view);
	//cube->lightCubeShader->setMat4("model", model);

	//unsigned int VAO3;
	//glGenVertexArrays(1, &VAO3);
	//VAO3 = cube->createCustomLightCube(0.2f, 0.2f, 0.2f, cube->pointLightPositions[0].x, cube->pointLightPositions[0].y, cube->pointLightPositions[0].z);
	//glBindVertexArray(VAO3);
	//glDrawArrays(GL_TRIANGLES, 0, 36*2);
	//glBindVertexArray(0);


	glUseProgram(0);
}

void RenderHandler::init(){

}



void RenderHandler::keyInput(int key, float speed){
	if (staticActiveBlockCollision(key)) std::cout << "collision" << std::endl;
	//up
	else {
		if (key == 0 && !activeBlockIsMoving)
			activeBlock_y += 1;
		//down
		if (key == 1 && !activeBlockIsMoving)
			activeBlock_y -= 1;
		//left
		if (key == 2 && !activeBlockIsMoving)
			activeBlock_x += 1;
		//right
		if (key == 3 && !activeBlockIsMoving)
			activeBlock_x -= 1;
	}
	//space:
	if (key == 4) {
		activeBlockIsMoving = true;
	}
	int temp_z = activeBlock_z;
	int grid_x = activeBlock_x + 3;
	int grid_y = activeBlock_y + 2;
	int gridLocation = (temp_z * 25) + ((grid_y - 1) * 5) + grid_x;
	std::cout << "grid location: " << gridLocation << " grid x: " << grid_x << " grid y: " << grid_y << std::endl;
}

void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
}







void RenderHandler::renderActiveBlock(){
	cube->shader->use();
	if (firstActiveBlockCall) {
		//function that adds previous block's location to the grid
		glGenVertexArrays(1, &blocksVAO);
		blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f);
		firstActiveBlockCall = false;
	}
	
	glBindVertexArray(blocksVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube->activeBlockTexture);
	model = glm::translate(model, glm::vec3(activeBlock_x, activeBlock_y, activeBlock_z));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//returns int main_state back to game (or whatever .cpp that has a pointer to this class)
int RenderHandler::render(int gamemode)
{
	/*switch (gamemode) {
	case 1:
		//first sort of gamemode. Includes a number of ifs dependent on input (+) and decides what int to return. The int that returns becomes the new main_state
		//int in the game.cpp program, and thus changes what switch-case will be entered on the next loop
		
		switch (gamemode1_state) {
		case 0: //gamemode1->init();
			gamemode1_state = 1;
			break;
		case 1: //render game
			break;
		case 2: //pause menu
			break;
		case 3: //dead
			break;
		case 4: //restart
			break;
		case 5: //exit gamemode1->~gamemode1(); / ~gamemode1;
		default:
			std::cout << "error" << std::endl;
			break;
		}

		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}*/
	return 1;
}

void RenderHandler::light()
{
	cube->shader->setVec3("lightPosition", cameraPos.x, cameraPos.y, cameraPos.z);
	cube->shader->setVec3("lightDirection", cameraFront.x, cameraFront.y, cameraFront.z);
	cube->shader->setFloat("lightCutOff", glm::cos(glm::radians(50.5f)));
	cube->shader->setFloat("lightOuterCutOff", glm::cos(glm::radians(70.5f)));
	cube->shader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	// light properties
	cube->shader->setVec3("lightAmbient", 0.1f, 0.1f, 0.1f);
	cube->shader->setVec3("lightDiffuse", 0.8f, 0.8f, 0.8f);
	cube->shader->setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);
	cube->shader->setFloat("lightConstant", 1.0f);
	cube->shader->setFloat("lightLinear", 0.09f);
	cube->shader->setFloat("lightQuadratic", 0.032f);

	// pointlight
	cube->shader->setVec3("pointLights[0].position", cube->pointLightPositions[0].x, cube->pointLightPositions[0].y, cube->pointLightPositions[0].z);
	cube->shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	cube->shader->setVec3("pointLights[0].diffuse", 0.1f, 0.1f, 0.1f);
	cube->shader->setVec3("pointLights[0].specular", 0.1f, 0.1f, 0.1f);
	cube->shader->setFloat("pointLights[0].constant", 0.1f);
	cube->shader->setFloat("pointLights[0].linear", 0.09f);
	cube->shader->setFloat("pointLights[0].quadratic", 0.032f);


	cube->shader->setFloat("materialShininess", 32.0f);
}

void RenderHandler::projectionInit() {
	//projection * view * model * aPos -> these three initiations could possibly be elsewhere
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	//levelShader uniform locations
	modelLoc = cube->shader->getUniformLoc("model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc = cube->shader->getUniformLoc("view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = cube->shader->getUniformLoc("projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void RenderHandler::moveActiveBlock(){
	if(!movingActiveBlockCollision())
		activeBlock_z += 2.5 * speed;
	else {
		//std::cout << "moving block stopped" << std::endl;
		newActiveBlock();
		activeBlockIsMoving = false;
	}
}

bool RenderHandler::movingActiveBlockCollision(){
	int temp_z = activeBlock_z;
	int grid_x = activeBlock_x + 3;
	int grid_y = activeBlock_y + 2;
	int gridLocation = (temp_z * 25) + ((grid_y - 1) * 5) + grid_x;
	if (grid[gridLocation + 25] == 1 && gridLocation < 275 - 23) {
		grid[gridLocation] = 1;
		std::cout << "collision with gridloc: " << gridLocation + 25 << std::endl;
		return true;
	}
	else 
		return false;
}

void RenderHandler::newActiveBlock(){
	block->activeToInactive(36, activeBlock_x, activeBlock_y, activeBlock_z);
	inactiveVAO = block->getInactiveBlocksVAO();
	blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f);
	//inactiveBlockVertices
	inactiveBlockVertices.push_back({});
	auto& vertex = inactiveBlockVertices.back();
	activeBlock_x = 0;
	activeBlock_y = 0;
	activeBlock_z = 0;
	//vertex.index = 1;
	vertex.vertices = 36;
	std::cout << "inactiveblock vertices: " << inactiveBlockVertices[0].vertices << std::endl;
	inactive = true;
}

bool RenderHandler::staticActiveBlockCollision(int key)
{
	int grid_x = getGridXLoc();
	int grid_y = getGridYLoc();
	switch (key) {
	case 0:
		if (grid_y >= 5) return true;
		else return false;
		break;
	case 1:
		if (grid_y <= 1) return true;
		else return false;
		break;

	case 2:
		if (grid_x >= 5) return true;
		else return false;
		break;

	case 3:
		if (grid_x <= 1) return true;
		else return false;
		break;

	default:
		std::cout << "not collision causing input " << std::endl;
		break;
	}
	return false;
}

void RenderHandler::renderInactiveBlock(){
	cube->shader->use();
	
	glBindVertexArray(inactiveVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube->inactiveTexture);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//model = glm::translate(model, glm::vec3(activeBlock_x, activeBlock_y, activeBlock_z));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	for (int i = 0; i < inactiveBlockVertices.size() - 1; i++) {
		//glDrawArrays(GL_TRIANGLES, inactiveBlockVertices[i].vertices, inactiveBlockVertices[i + 1].vertices);
		glDrawArrays(GL_TRIANGLES, 0, 36 * 40);
	}
	//light();
	//transformations();
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

int RenderHandler::getGridXLoc()
{
	int grid_x = activeBlock_x + 3;
	return grid_x;
}

int RenderHandler::getGridYLoc()
{
	int grid_y = activeBlock_y + 2;
	return grid_y;
}


void RenderHandler::updateSpeed(float gamespeed){
	speed = gamespeed;
}

void RenderHandler::cameraInit() {
	cameraPos = glm::vec3(1.68f, 2.5f, -2.74f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void RenderHandler::transformations() {
	// view/projection transformations
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	cube->shader->setMat4("projection", projection);
	cube->shader->setMat4("view", view);

	// world transformation
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cube->shader->setMat4("model", model);
}