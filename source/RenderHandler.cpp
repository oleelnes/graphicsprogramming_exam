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
	for (int i = 0; i < 4; i++) {
		activeBlockLocation.push_back({});
		auto& vertex = activeBlockLocation.back();
		vertex.location = { 0.0f, 0.0f, 0.0f };
	}
	inactiveBlockVertices.push_back({});
	auto& vertex = inactiveBlockVertices.back();
	vertex.type = 0; // what type of block
	vertex.vertices = 0; // how many vertices to draw
	vertex.layer = 10; //sentinel key
	vertex.verticeMultiplier = 1;
	solidBlocks = 0; //counter that is used to access the various entries of the inactiveBlockVertices-vector/solidBlocks-vector
	gridZLoc = 0;
	drawcallcounter = 0;
	activeBlockType = 0;
	lastActiveBlockType = 0;
	activeBlocks = 1;
	input = false;
	yaw = 0;
	roll = 0;
	pitch = 0;

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
			activeBlock_y += 1; input = true;
		//down
		if (key == 1 && !activeBlockIsMoving)
			activeBlock_y -= 1; input = true;
		//left
		if (key == 2 && !activeBlockIsMoving)
			activeBlock_x += 1; input = true;
		//right
		if (key == 3 && !activeBlockIsMoving)
			activeBlock_x -= 1; input = true;

		//ROTATION INPUT:
		// yaw +1
		if (key == 4 && !activeBlockIsMoving && canRotate()) {
			roll++;
			if (roll >= 4) roll = 0;
			input = true;
		}
		//yaw -1
		if (key == 5 && !activeBlockIsMoving && canRotate()) {
			roll--;
			if (roll <= -1) roll = 3;
			input = true;
		}
		//pitch +1
		if (key == 6 && !activeBlockIsMoving) {

		}

		//pitch -1
		if (key == 7 && !activeBlockIsMoving) {

		}

		//roll +1
		if (key == 8 && !activeBlockIsMoving) {

		}

		//roll -1
		if (key == 9 && !activeBlockIsMoving) {

		}


	}
	//space:
	if (key == 10) {
		activeBlockIsMoving = true;
	}
}

void RenderHandler::activeBlockRotation(int yaw, int pitch, int roll) {
	std::vector<ActiveBlockPos> tempVec;
	for (int i = 0; i < 4; i++) {
		tempVec.push_back({});
		auto& vertex = tempVec.back();
		vertex.location = { 0.0f, 0.0f, 0.0f };
	}
	for (int r = 0; r < roll; r++) {
		for (int i = 0; i < 4; i++) {
			tempVec[i].location = { activeBlock_x + ((activeBlock_y - activeBlockLocation[i].location.y)), activeBlock_y - (activeBlock_x - activeBlockLocation[i].location.x), activeBlockLocation[i].location.z };
			std::cout << activeBlock_y << std::endl;
			activeBlockLocation[i].location = tempVec[i].location;
		}
	}
	
}

void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
}

void RenderHandler::renderActiveBlock(){
	cube->shader->use();
	if (firstActiveBlockCall) {
		//function that adds previous block's location to the grid
		//glGenVertexArrays(1, &blocksVAO);
		
		lastActiveBlockType = activeBlockType;
		activeBlockType = rand() % 4; //this gives us either 0, 1, 2 or 3
		if (activeBlockType == lastActiveBlockType) {
			if (activeBlockType > 2) activeBlockType--;
			else activeBlockType++;
		}
		if (activeBlockType > 3 || activeBlockType < 0) activeBlockType = 2;
		std::cout << "active block type: " << activeBlockType << std::endl;

		//TODO: These ifs are redunant - fix it when rotation is done
		if (activeBlockType == 0) {
			activeBlocks = 1;
			blocksVAO = block->newActiveBlock(0, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
			firstActiveBlockCall = false;
			std::cout << " in 0 " <<  std::endl;
		}
		else if (activeBlockType == 1) { //L
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
			firstActiveBlockCall = false;
			std::cout << " in 1 " << std::endl;

			
		}
		else if (activeBlockType == 2) { //Z
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(2, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
			firstActiveBlockCall = false;
			std::cout << " in 2 " << std::endl;
		}
		else { //T
			activeBlocks = 4;
			blocksVAO = block->newActiveBlock(3, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
			firstActiveBlockCall = false;
			std::cout << " in 3 " << std::endl;
		}
	}
	if (input) {
		//glGenVertexArrays(1, &blocksVAO);
		blocksVAO = block->newActiveBlock(activeBlockType, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
		input = false;
	}
	glBindVertexArray(blocksVAO);
	glBindTexture(GL_TEXTURE_2D, texture->activeBlockTexture);
	//enables moving through translating position
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.1f));
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
			glDrawArrays(GL_TRIANGLES, inactiveBlockVertices[i - 1].vertices, 36 * inactiveBlockVertices[i].verticeMultiplier);
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
	bool tempTrue = false;
	for (int i = 0; i < activeBlocks; i++) {
		updateActiveBlockPos(activeBlockType);
		int temp_z = activeBlock_z;
		int grid_x = getGridXLoc(i); //TODO: Change these to be equal to getGridLocX() and y
		int grid_y = getGridYLoc(i);
		int gridLocation = (temp_z * 25) + ((grid_y - 1) * 5) + grid_x;
		if (grid[gridLocation + 25] == 1 && gridLocation < 275 - 23) {
			//grid[gridLocation] = 1;
			std::cout << "type " << activeBlockType << " collided and gridloc is " << gridLocation << " and grid of i is " << grid[gridLocation] << " and i is " << i << std::endl;
			setGridZLoc(gridLocation);
			tempTrue = true;
		}
	}
	if (tempTrue) {
		for (int i = 0; i < activeBlocks; i++) {
			updateActiveBlockPos(activeBlockType);
			int temp_z = activeBlock_z;
			int grid_x = getGridXLoc(i); //TODO: Change these to be equal to getGridLocX() and y
			int grid_y = getGridYLoc(i);
			int gridLocation = (temp_z * 25) + ((grid_y - 1) * 5) + grid_x;
			grid[gridLocation] = 1;
		}
	}
	if (tempTrue) { firstActiveBlockCall = true; return true;  }
	else return false;
}

/*
	TODO: make it newActiveBlock(int type) where type corresponds to how many vertices and what type will be added to the inactiveBlockVertices vector
	This function both resets the active block to its original position AND adds the previous activeBlock to the inactiveBlockVAO in Block.cpp (?)
*/
void RenderHandler::newActiveBlock(){
	//blocksVAO = block->newActiveBlock(1, 1.0f, 1.0f, 1.0f, roll, pitch, yaw);
	//std::cout << "active z: " << activeBlock_z << " gridlocz: " << gridZLoc << std::endl;
	glDeleteVertexArrays(1, &inactiveVAO);
	glGenVertexArrays(1, &inactiveVAO);
	inactiveVAO = block->activeToInactive(activeBlockType, activeBlock_x, activeBlock_y, activeBlock_z);

	solidBlocks++;
	inactiveBlockVertices.push_back({});
	auto& vertex = inactiveBlockVertices.back();
	vertex.type = 1;
	vertex.layer = gridZLoc;
	vertex.vertices = inactiveBlockVertices[solidBlocks - 1].vertices + 36 * activeBlocks;
	vertex.verticeMultiplier = activeBlocks;
	activeBlock_x = 0;  activeBlock_y = 0; activeBlock_z = 0; roll = 0; pitch = 0; yaw = 0;
	inactive = true;
}

bool RenderHandler::staticActiveBlockCollision(int key)
{
	int grid_x = getGridXLoc(0);
	int grid_y = getGridYLoc(0);
	//std::cout << "activeblocktype: " << activeBlockType << " active blocks: " << activeBlocks <<  std::endl;
	for (int i = 0; i < activeBlocks; i++) {
		updateActiveBlockPos(activeBlockType);
		int grid_x = getGridXLoc(i);
		int grid_y = getGridYLoc(i);
		switch (key) {
		case 0: //UP
			if (grid_y >= 5) return true;
			break;
		case 1: //DOWN
			if (grid_y <= 1) return true;
			break;

		case 2: //RIGHT
			if (grid_x >= 5) return true;
			break;

		case 3: //LEFT
			if (grid_x <= 1) return true;
			break;

		default:
			break;
		}
	}
	return false;
}

bool RenderHandler::canRotate() {
	for (int i = 0; i < activeBlocks; i++) {
		int grid_x = getGridXLoc(i);
		int grid_y = getGridYLoc(i);
		if (grid_y >= 5) return false;
		if (grid_y <= 1) return false;
		if (grid_x >= 5) return false;
		if (grid_x <= 1) return false;
	}
	return true;
}


int RenderHandler::getGridXLoc(int block)
{
	//int grid_x = activeBlock_x + 3;
	int grid_x = activeBlockLocation[block].location.x + 3;
	return grid_x;
}

int RenderHandler::getGridYLoc(int block)
{
	//int grid_y = activeBlock_y + 2;
	int grid_y = activeBlockLocation[block].location.y + 2;
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

void RenderHandler::updateActiveBlockPos(int type){
	switch (type) {
	case 0:
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[1].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockRotation(yaw, pitch, roll);
		break;
	case 1: //L Block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x + 1.0f,	activeBlock_y - 1.0f,	activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y - 1.0f,	activeBlock_z };
		activeBlockRotation(yaw, pitch, roll);
		break;
	case 2: //Z Block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x - 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x + 1.0f,	activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockRotation(yaw, pitch, roll);
		break;
	case 3: //T block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x - 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x + 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockRotation(yaw, pitch, roll);
		break;

	default:
		break;
	}
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
		//activeBlock_x = 0;  activeBlock_y = 0; activeBlock_z = 0;
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