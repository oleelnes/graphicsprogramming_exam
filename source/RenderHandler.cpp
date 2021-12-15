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
	texture = new TextureLoader();
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
	vertex.type = 0; // what type of block
	vertex.vertices = 0; // how many vertices to draw
	vertex.layer = 10; //sentinel key
	solidBlocks = 0; //counter that is used to access the various entries of the inactiveBlockVertices-vector/solidBlocks-vector
	gridZLoc = 0;
	drawcallcounter = 0;
	activeBlockType = 0;
	lastActiveBlockType = 0;
	activeBlocks = 1;

	srand(time(NULL)); rand(); //initiating random sequence and throwing away first random value
}

void RenderHandler::renderer(){
	

	cube->shader->use();
	light();
	transformations();

	//creating tunnel
	glBindVertexArray(tunnelVAO);
	cube->shader->setInt("withTexture", 1);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->gridTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6*(((5*10)*4)+(5*5))); //endret til 6
	glBindVertexArray(0);
	glUseProgram(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//cube->shader->setInt("withTexture", 0);


	if (inactive) renderAllSolidBlocks();
	if (activeBlockIsMoving) moveActiveBlock();
	renderActiveBlock();
	

	glBindVertexArray(0);

	glUseProgram(0);
}

void RenderHandler::init(){

}



void RenderHandler::keyInput(int key, float speed){
	if (staticActiveBlockCollision(key));//std::cout << "collision" << std::endl;
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
}

void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
}

void RenderHandler::renderActiveBlock(){
	cube->shader->use();
	if (firstActiveBlockCall) {
		//function that adds previous block's location to the grid
		glGenVertexArrays(1, &blocksVAO);
		lastActiveBlockType = activeBlockType;
		activeBlockType = rand() % 4; //this gives us either 0, 1, 2 or 3
		if (activeBlockType == lastActiveBlockType) {
			if (activeBlockType > 2) activeBlockType--;
			else activeBlockType++;
		}
		if (activeBlockType > 3 || activeBlockType < 0) activeBlockType = 2;
		std::cout << "active block type: " << activeBlockType << std::endl;
		if (activeBlockType == 0) {
			activeBlocks = 1;
			blocksVAO = block->newActiveBlock(0, 1.0f, 1.0f, 1.0f);
			firstActiveBlockCall = false;
			std::cout << " in 0 " <<  std::endl;
		}
		else if (activeBlockType == 1) { //L
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f);
			firstActiveBlockCall = false;
			std::cout << " in 1 " << std::endl;
		}
		else if (activeBlockType == 2) { //Z
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(2, 1.0f, 1.0f, 1.0f);
			firstActiveBlockCall = false;
			std::cout << " in 2 " << std::endl;
		}
		else { //T
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(3, 1.0f, 1.0f, 1.0f);
			firstActiveBlockCall = false;
			std::cout << " in 3 " << std::endl;
		}
	}
	glBindVertexArray(blocksVAO);
	glBindTexture(GL_TEXTURE_2D, texture->activeBlockTexture);
	//enables moving through translating position
	model = glm::translate(model, glm::vec3(activeBlock_x, activeBlock_y, activeBlock_z));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//drawing
	glDrawArrays(GL_TRIANGLES, 0, 36 * activeBlocks);
	glBindVertexArray(0);
	glUseProgram(0);
}


void RenderHandler::renderAllSolidBlocks(){
	drawcallcounter = 0;
	renderInactiveBlock(10, texture->layer_10);
	renderInactiveBlock(9, texture->layer_9);
	renderInactiveBlock(8, texture->layer_8);
	renderInactiveBlock(7, texture->layer_7);
	renderInactiveBlock(6, texture->layer_6);
	renderInactiveBlock(5, texture->layer_5);
	renderInactiveBlock(4, texture->layer_4);
	renderInactiveBlock(3, texture->layer_3);
	renderInactiveBlock(2, texture->layer_2);
}

/*
	This function is called by
*/
void RenderHandler::renderInactiveBlock(int requestedLayer, GLuint& layer) {
	cube->shader->use();

	glBindVertexArray(inactiveVAO);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, layer);
	
	for (int i = 1; i <= solidBlocks; i++) {
		if (inactiveBlockVertices[i].layer == requestedLayer) {
			//std::cout << "requested layer/actual layer: " << inactiveBlockVertices[i].layer << " / " << requestedLayer << std::endl;
			//glBindTexture(GL_TEXTURE_2D, layer);
			glDrawArrays(GL_TRIANGLES, inactiveBlockVertices[i - 1].vertices, 36);
			drawcallcounter++;
			//std::cout << "drawcalls: " << drawcallcounter << std::endl;
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);

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


void RenderHandler::moveActiveBlock(){
	if(!movingActiveBlockCollision())
		activeBlock_z += 5.5 * speed;
	else {
		//std::cout << "moving block stopped" << std::endl;
		newActiveBlock();
		activeBlock_x = 0;  activeBlock_y = 0; activeBlock_z = 0;
		activeBlockIsMoving = false;
	}
}

bool RenderHandler::movingActiveBlockCollision(){
	int temp_z = activeBlock_z;
	int grid_x = activeBlock_x + 3; //TODO: Change these to be equal to getGridLocX() and y
	int grid_y = activeBlock_y + 2;
	int gridLocation = (temp_z * 25) + ((grid_y - 1) * 5) + grid_x;
	if (grid[gridLocation + 25] == 1 && gridLocation < 275 - 23) {
		grid[gridLocation] = 1;
		setGridZLoc(gridLocation);
		firstActiveBlockCall = true;
		//std::cout << "collision with gridloc: " << gridLocation + 25 << std::endl;
		return true;
	}
	else 
		return false;
}

/*
	TODO: make it newActiveBlock(int type) where type corresponds to how many vertices and what type will be added to the inactiveBlockVertices vector
	This function both resets the active block to its original position AND adds the previous activeBlock to the inactiveBlockVAO in Block.cpp (?)
*/
void RenderHandler::newActiveBlock(){
	blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f);
	//std::cout << "active z: " << activeBlock_z << " gridlocz: " << gridZLoc << std::endl;
	glDeleteVertexArrays(1, &inactiveVAO);
	glGenVertexArrays(1, &inactiveVAO);
	inactiveVAO = block->activeToInactive(36, activeBlock_x, activeBlock_y, activeBlock_z);

	solidBlocks++;
	inactiveBlockVertices.push_back({});
	auto& vertex = inactiveBlockVertices.back();
	vertex.type = 1;
	vertex.layer = gridZLoc;
	vertex.vertices = inactiveBlockVertices[solidBlocks - 1].vertices + 36;

	//activeBlock_x = 0;  activeBlock_y = 0; activeBlock_z = 0; //setting the activeBlock locations to 0
	//std::cout << "inactiveblock vertices: " << inactiveBlockVertices[solidBlocks].vertices << std::endl;
	//std::cout << "vertex layer of solid block " << solidBlocks << " is " << inactiveBlockVertices[solidBlocks].layer << " and solidblocks is: " << solidBlocks << std::endl;
	//std::cout << "vertices at "<< solidBlocks << " is: " << inactiveBlockVertices[solidBlocks].vertices << std::endl;
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
		//std::cout << "not collision causing input " << std::endl;
		break;
	}
	return false;
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


void RenderHandler::setGridZLoc(int currGridLoc){
	if (currGridLoc <= 275 && currGridLoc > 250) gridZLoc = 11;
	else if (currGridLoc <= 250 && currGridLoc > 225) gridZLoc = 10;
	else if (currGridLoc <= 225 && currGridLoc > 200) gridZLoc = 9;
	else if (currGridLoc <= 200 && currGridLoc > 175) gridZLoc = 8;
	else if (currGridLoc <= 175 && currGridLoc > 150) gridZLoc = 7;
	else if (currGridLoc <= 150 && currGridLoc > 125) gridZLoc = 6;
	else if (currGridLoc <= 125 && currGridLoc > 100) gridZLoc = 5;
	else if (currGridLoc <= 100 && currGridLoc > 75) gridZLoc = 4;
	else if (currGridLoc <= 75 && currGridLoc > 50) gridZLoc = 3;
	else if (currGridLoc <= 50 && currGridLoc > 25) gridZLoc = 2;
	else if (currGridLoc <= 25 && currGridLoc > 0) gridZLoc = 1;
	//std::cout << "grid location " << currGridLoc << " gave us a gridZLoc of " << gridZLoc << std::endl;
}


void RenderHandler::updateSpeed(float gamespeed){
	speed = gamespeed;
}

void RenderHandler::updateTime(float time)
{
}

void RenderHandler::descend(){
	activeBlock_z += 1.0f;
	if (movingActiveBlockCollision()) {
		newActiveBlock();
		activeBlock_x = 0;  activeBlock_y = 0; activeBlock_z = 0;
		activeBlockIsMoving = false;
	}
}

int RenderHandler::getScore()
{
	return 0;
}

int RenderHandler::getLayer()
{
	return 0;
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