#include "RenderHandler.h"

/*
	This class creates pointers to:
		* Shader (main_shader)
		* Overlay (on-screen and real-time informative text such as score, health, stamina.
	The Shader-pointer is sent to the functions in the draw-functions in the gamemode-classes.
*/

RenderHandler::RenderHandler(){
	cube = new Cube();
	cameraInit();
	projectionInit();
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //initiating the camera. 
}

void RenderHandler::renderer(){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube->shader->use();
	//glBindVertexArray(cube->cubeVAO);
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	light();
	transformations();
	
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	unsigned int tunnelVAO;
	glGenVertexArrays(1, &tunnelVAO);
	tunnelVAO = cube->createCubeTunnel(5, 5, 10);
	//ournewVAO += cube->createCustomCube(100.0f, 0.1f, 100.0f, 3.5f);
	glBindVertexArray(tunnelVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36*(((5*10)*4)+(5*5)));
	glBindVertexArray(0);

	unsigned int ournewVAO;
	glGenVertexArrays(1, &ournewVAO);
	ournewVAO = cube->createCustomCube(2.0f, 0.5f, 2.0f, -0.8f);
	//ournewVAO += cube->createCustomCube(100.0f, 0.1f, 100.0f, 3.5f);
	glBindVertexArray(ournewVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawArrays(GL_QUADS, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);

	cube->lightCubeShader->use();
	cube->lightCubeShader->setMat4("projection", projection);
	cube->lightCubeShader->setMat4("view", view);
	cube->lightCubeShader->setMat4("model", model);
	unsigned int VAO3;
	glGenVertexArrays(1, &VAO3);
	VAO3 = cube->createCustomLightCube(0.2f, 0.2f, 0.2f, cube->pointLightPositions[0].x, cube->pointLightPositions[0].y, cube->pointLightPositions[0].z);
	glBindVertexArray(VAO3);
	glDrawArrays(GL_TRIANGLES, 0, 36*2);
	glBindVertexArray(0);


	glUseProgram(0);
}

void RenderHandler::init(){

}

void RenderHandler::projectionInit(){
	//projection * view * model * aPos -> these three initiations could possibly be elsewhere
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(85.0f), 800.0f / 800.0f, 0.1f, 100.0f);

	//levelShader uniform locations
	modelLoc = cube->shader->getUniformLoc("model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc = cube->shader->getUniformLoc("view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = cube->shader->getUniformLoc("projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void RenderHandler::cameraInit(){
	cameraPos = glm::vec3(3.0f, 3.0f, 5.f);
	cameraFront = glm::vec3(0.0f, -1.f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void RenderHandler::transformations(){
	// view/projection transformations
	projection = glm::perspective(glm::radians(85.0f), 800.0f / 800.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	cube->shader->setMat4("projection", projection);
	cube->shader->setMat4("view", view);

	// world transformation
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cube->shader->setMat4("model", model);
}

void RenderHandler::keyInput(int key, float speed){
	if (key == 0)
		cameraPos += speed * cameraFront;
	//down
	if (key == 1)
		cameraPos -= speed * cameraFront;
	//left
	if (key == 2)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	//right
	if (key == 3)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
}

void RenderHandler::light()
{
	cube->shader->setVec3("lightPosition", cameraPos.x, cameraPos.y, cameraPos.z);
	cube->shader->setVec3("lightDirection", cameraFront.x, cameraFront.y, cameraFront.z);
	cube->shader->setFloat("lightCutOff", glm::cos(glm::radians(34.5f)));
	cube->shader->setFloat("lightOuterCutOff", glm::cos(glm::radians(42.5f)));
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
	cube->shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	cube->shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	cube->shader->setFloat("pointLights[0].constant", 1.0f);
	cube->shader->setFloat("pointLights[0].linear", 0.09f);
	cube->shader->setFloat("pointLights[0].quadratic", 0.032f);


	cube->shader->setFloat("materialShininess", 32.0f);
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


