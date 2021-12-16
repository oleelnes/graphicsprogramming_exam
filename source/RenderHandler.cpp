#include "RenderHandler.h"

/*
	This class is responsible for rendering the game scene 
	This class creates pointers to:
		* Block class
		* TextureLoader class
		* Tunnel class
	The Shader-pointer is sent to the functions in the draw-functions in the gamemode-classes.
*/


//Constructor
RenderHandler::RenderHandler(){
	init();
}

/*
	This function is the root function that initiates all other render-related functions of this class
*/
void RenderHandler::renderer(){
	tunnel->shader->use();
	light();
	transformations();

	//rendering tunnel
	renderTunnel();

	//render all solid blocks
	if (inactive) renderAllSolidBlocks();

	//if active block is currently falling/moving, move active block
	if (activeBlockIsMoving) moveActiveBlock();

	//if !gameover, render the activeBlock
	if (!gameOver) renderActiveBlock();
	
	//function that 
	setGameOver();

	glBindVertexArray(0);

	glUseProgram(0);
}

//Function that renders the tunnel
void RenderHandler::renderTunnel() {
	glBindVertexArray(tunnelVAO);
	tunnel->shader->setInt("withTexture", 1);
	glBindTexture(GL_TEXTURE_2D, texture->gridTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6 * (((5 * 10) * 4) + (5 * 5))); //endret til 6
	glBindVertexArray(0);
	glUseProgram(0);
}

/*
	This function decides what happens when a certain key is pushed
	int key: the key that has been pushed
*/
void RenderHandler::keyInput(int key){
	if (staticActiveBlockCollision(key));//std::cout << "collision" << std::endl;
	//up
	else if (!gameOver){
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
		// roll +1
		if (key == 4 && !activeBlockIsMoving && canRotate()) {
			roll++;
			if (roll >= 4) roll = 0;
			input = true;
		}
		//roll -1
		if (key == 5 && !activeBlockIsMoving && canRotate()) {
			roll--;
			if (roll <= -1) roll = 3;
			input = true;
		}
		//pitch +1
		if (key == 6 && !activeBlockIsMoving && canRotate()) {
			pitch++;
			if (pitch >= 4) pitch = 0;
			input = true;
		}

		//pitch -1
		if (key == 7 && !activeBlockIsMoving && canRotate()) {
			pitch--;
			if (pitch <= -1) pitch = 3;
			input = true;
		}

		//yaw +1
		if (key == 8 && !activeBlockIsMoving && canRotate()) {
			yaw++;
			if (yaw >= 4) yaw = 0;
			input = true;
		}

		//yaw -1
		if (key == 9 && !activeBlockIsMoving && canRotate()) {
			yaw--;
			if (yaw <= -1) yaw = 3;
			input = true;
		}
		//space:
		if ( key == 10) {
			activeBlockIsMoving = true;
		}
	}
}

/*
	This function adjusts the location of the active block depending on rotation
*/
void RenderHandler::activeBlockRotation() {
	std::vector<ActiveBlockPos> tempVec;
	for (int i = 0; i < 4; i++) {
		tempVec.push_back({});
		auto& vertex = tempVec.back();
		vertex.location = { 0.0f, 0.0f, 0.0f };
	}
	for (int r = 0; r < roll; r++) {
		for (int i = 0; i < 4; i++) {
			tempVec[i].location = { activeBlock_x + ((activeBlock_y - activeBlockLocation[i].location.y)), 
				activeBlock_y - (activeBlock_x - activeBlockLocation[i].location.x), 
				activeBlockLocation[i].location.z };
			activeBlockLocation[i].location = tempVec[i].location;
		}
	}
	
}

void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
}

bool RenderHandler::getGameOver()
{
	return gameOver;
}

/*
	This function renders the active block
*/
void RenderHandler::renderActiveBlock(){
	tunnel->shader->use();
	//when a new active block is first called, the underneath if-statement generated a new activeBlocksVAO with 
	//a new randomly selected type of block
	if (firstActiveBlockCall) {
		lastActiveBlockType = activeBlockType;
		activeBlockType = rand() % 4; //this gives us either 0, 1, 2 or 3
		if (activeBlockType == lastActiveBlockType) {
			if (activeBlockType > 2) activeBlockType--;
			else activeBlockType++;
		}
		if (activeBlockType > 3 || activeBlockType < 0) activeBlockType = 2;


		if (activeBlockType == 0) activeBlocks = 1;
		else activeBlocks = 4;
		activeBlocksVAO = block->newActiveBlock(activeBlockType, 1.0f, 1.0f, activeBlock_z, roll, pitch, yaw);
		firstActiveBlockCall = false;
	}
	if (input) {
		activeBlocksVAO = block->newActiveBlock(activeBlockType, 1.0f, 1.0f, activeBlock_z, roll, pitch, yaw);
		input = false;
	}
	glBindVertexArray(activeBlocksVAO);
	glBindTexture(GL_TEXTURE_2D, texture->activeBlockTexture);
	//enables moving through translating position
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(activeBlock_x, activeBlock_y, activeBlock_z));
	//model = glm::rotate(model, glm::radians(90.0f * pitch), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(90.0f * yaw), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//drawing
	glDrawArrays(GL_TRIANGLES, 0, 36 * activeBlocks);
	glBindVertexArray(0);
	glUseProgram(0);
}

/*
	This function's purpose is to ensure the rendering of all solid blocks
*/
void RenderHandler::renderAllSolidBlocks(){
	drawcallcounter = 0;
	renderSolidBlock(10, texture->layer_10);
	renderSolidBlock(9, texture->layer_9);
	renderSolidBlock(8, texture->layer_8);
	renderSolidBlock(7, texture->layer_7);
	renderSolidBlock(6, texture->layer_6);
	renderSolidBlock(5, texture->layer_5);
	renderSolidBlock(4, texture->layer_4);
	renderSolidBlock(3, texture->layer_3);
	renderSolidBlock(2, texture->layer_2);
}

/*
	This function is called by renderAllSolidBlocks
	int requestedLayer: the layer to draw blocks at 
	GLuint& layer: a reference to a layer-specific texture
*/
void RenderHandler::renderSolidBlock(int requestedLayer, GLuint& layer) {
	tunnel->shader->use();
	glBindVertexArray(solidBlocksVAO);
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, layer);
	for (int i = 1; i <= solidBlocks; i++) {
		if (solidBlockVertices[i].layer == requestedLayer) {
			glDrawArrays(GL_TRIANGLES, solidBlockVertices[i - 1].vertices, 36 * solidBlockVertices[i].verticeMultiplier);
			drawcallcounter++;
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);

}

/*
	This function moves an active block given that the active block doesn't collide with the tunnel far-plane or any solid blocks,
	if a crash is detected, the moving stops and newActiveBlock() is called
*/
void RenderHandler::moveActiveBlock(){
	if(!movingActiveBlockCollision())
		activeBlock_z += 5.5 * speed;
	else {
		//std::cout << "moving block stopped" << std::endl;
		newActiveBlock();
		activeBlock_x = 2;  activeBlock_y = 1; activeBlock_z = 0;
		activeBlockIsMoving = false;
	}
}

/*
	This function checks whether a moving active block (space has been pressed) is crashing with either a the far tunnel wall or 
	a solid block 
	This function is also called every time a static(-ish) active block descends one block further down when seven seconds have passed without 
	submitting the block
*/
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
	Function that, when called, descends a static(-ish) active block one block further down the tunnel
	This function is called from the game.class
*/
void RenderHandler::descend() {
	activeBlock_z += 1.0f;
	if (movingActiveBlockCollision()) {
		newActiveBlock();
		activeBlockIsMoving = false;
	}
}

/*
	This function both resets the active block to its original position AND adds the previous activeBlock to the solidBlocksVAO in Block.cpp (?)
	Is called by moveActiveBlock() function
*/
void RenderHandler::newActiveBlock(){
	glDeleteVertexArrays(1, &solidBlocksVAO);
	glGenVertexArrays(1, &solidBlocksVAO);
	solidBlocksVAO = block->activeToSolid(activeBlockType, activeBlock_x, activeBlock_y, activeBlock_z);

	solidBlocks++;
	solidBlockVertices.push_back({});
	auto& vertex = solidBlockVertices.back();
	vertex.type = 1;
	vertex.layer = gridZLoc;
	vertex.vertices = solidBlockVertices[solidBlocks - 1].vertices + 36 * activeBlocks;
	vertex.verticeMultiplier = activeBlocks;
	if (gridZLoc < layer) layer = gridZLoc;
	activeBlock_x = 2;  activeBlock_y = 1; activeBlock_z = 0; roll = 0; pitch = 0; yaw = 0;
	inactive = true;
	score += 5 * activeBlocks;
}

/*
	This function checks whether the active block is colliding with the walls when it's still static(-ish)
	int key: what arrow key is pushed, in order to know where to look
*/
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

//Function that checks whether a rotation is possible -> not fully completed
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
	int grid_x = activeBlockLocation[block].location.x + 1; //+3
	return grid_x;
}

int RenderHandler::getGridYLoc(int block)
{
	int grid_y = activeBlockLocation[block].location.y + 1; //+2
	return grid_y;
}

/*
	This function sets the current z position of the active block
	int currGridLoc: the current gridlocation of the active block.
*/
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

/*
	This function updates the active block's location data
	int type: the shape of the active block
*/
void RenderHandler::updateActiveBlockPos(int type){
	switch (type) {
	case 0:
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[1].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y,			activeBlock_z };
		activeBlockRotation();
		break;
	case 1: //L Block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x + 1.0f,	activeBlock_y - 1.0f,	activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y - 1.0f,	activeBlock_z };
		activeBlockRotation();
		break;
	case 2: //Z Block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x - 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x + 1.0f,	activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockRotation();
		break;
	case 3: //T block
		activeBlockLocation[0].location = { activeBlock_x,			activeBlock_y,			activeBlock_z }; //center
		activeBlockLocation[1].location = { activeBlock_x - 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[2].location = { activeBlock_x + 1.0f,	activeBlock_y,			activeBlock_z };
		activeBlockLocation[3].location = { activeBlock_x,			activeBlock_y + 1.0f,	activeBlock_z };
		activeBlockRotation();
		break;

	default:
		break;
	}
}


void RenderHandler::updateSpeed(float gamespeed){
	speed = gamespeed;
}




//returns game score
int RenderHandler::getScore()
{
	return score;
}

//returns the closest layer (to the camera) that has been reached
int RenderHandler::getLayer()
{
	return layer;
}


/*
	RenderHandler's init function -> initializes a bunch of necessary variables, pointer, vectors, etc.
*/
void RenderHandler::init() {
	tunnel = new Tunnel();
	block = new Block();
	texture = new TextureLoader();
	cameraInit();
	projectionInit();
	//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //initiating the camera. 
	firstActiveBlockCall = true;
	activeBlockIsMoving = false;
	inactive = false;
	glGenVertexArrays(1, &tunnelVAO);
	tunnelVAO = tunnel->createCubeTunnel(5, 5, 10);
	activeBlock_x = 2;
	activeBlock_y = 1;
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
	solidBlockVertices.push_back({});
	auto& vertex = solidBlockVertices.back();
	vertex.type = 0; // what type of block
	vertex.vertices = 0; // how many vertices to draw
	vertex.layer = 10; //bottom layer
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
	score = 0;
	layer = 10;
	srand(time(NULL)); rand(); //initiating random sequence and throwing away first random value
}


/*
	Source 4 (See readme for full referencing)
*/
void RenderHandler::cameraInit() {
	cameraPos = glm::vec3(1.68f, 2.5f, -2.74f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

/*
	Source 4 (See readme for full referencing)
*/
void RenderHandler::transformations() {
	// view/projection transformations
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	tunnel->shader->setMat4("projection", projection);
	tunnel->shader->setMat4("view", view);

	// world transformation
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tunnel->shader->setMat4("model", model);
}

/*
	Source 4 (See readme for full referencing)
*/
void RenderHandler::light()
{
	tunnel->shader->setVec3("lightPosition", cameraPos.x, cameraPos.y, cameraPos.z);
	tunnel->shader->setVec3("lightDirection", cameraFront.x, cameraFront.y, cameraFront.z);
	tunnel->shader->setFloat("lightCutOff", glm::cos(glm::radians(50.5f)));
	tunnel->shader->setFloat("lightOuterCutOff", glm::cos(glm::radians(70.5f)));
	tunnel->shader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	// light properties
	tunnel->shader->setVec3("lightAmbient", 0.1f, 0.1f, 0.1f);
	tunnel->shader->setVec3("lightDiffuse", 0.8f, 0.8f, 0.8f);
	tunnel->shader->setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);
	tunnel->shader->setFloat("lightConstant", 1.0f);
	tunnel->shader->setFloat("lightLinear", 0.09f);
	tunnel->shader->setFloat("lightQuadratic", 0.032f);

	// pointlight
	tunnel->shader->setVec3("pointLights[0].position", tunnel->pointLightPositions[0].x, tunnel->pointLightPositions[0].y, tunnel->pointLightPositions[0].z);
	tunnel->shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	tunnel->shader->setVec3("pointLights[0].diffuse", 0.1f, 0.1f, 0.1f);
	tunnel->shader->setVec3("pointLights[0].specular", 0.1f, 0.1f, 0.1f);
	tunnel->shader->setFloat("pointLights[0].constant", 0.1f);
	tunnel->shader->setFloat("pointLights[0].linear", 0.09f);
	tunnel->shader->setFloat("pointLights[0].quadratic", 0.032f);


	tunnel->shader->setFloat("materialShininess", 32.0f);
}

/*
	Source 4 (See readme for full referencing)
*/
void RenderHandler::projectionInit() {
	//projection * view * model * aPos -> these three initiations could possibly be elsewhere
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	//levelShader uniform locations
	modelLoc = tunnel->shader->getUniformLoc("model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc = tunnel->shader->getUniformLoc("view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = tunnel->shader->getUniformLoc("projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void RenderHandler::setGameOver(){
	if (layer == 1) gameOver = true;
	else gameOver = false;
}
