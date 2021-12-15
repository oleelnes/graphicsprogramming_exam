#include "game.h"

/*
	This class creates a window and holds the while-loop for the game
	* This class creates two pointers:
		- To the class window, which is used to create the window
		- To the class GameRenderer, which is used to render the game
	* 
*/

/*
This method initializes all the necessary pointers and functions.
*/
void game::gameInit(){
	gameWindow->windowCreator();
	renderHandler = new RenderHandler();
	buttonPressed = false;
	
}



void game::run(){
	gameInit();
	
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(gameWindow->winWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	//glfwWindowHint(GLFW_SAMPLES, 12);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	int gamemode = 0;
	int main_state = 0;
	bool pauseMenu = false;

	time = 0.0f;
	timeBuffer = 0.0f;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(gameWindow->winWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		float currentFrame = glfwGetTime();
		
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		renderHandler->updateSpeed(2.5 * deltaTime);
		//renderHandler->renderer();
		
		
		//ui.inGameStats(1, 2);
		//std::cout << "shello" << std::endl;
		
		switch (main_state) {
		case 0: //menu -- decides int gamemode and changes main_state to 1 or 2 depending on whether a gamemode has been selected or if exit game has been selected
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (pauseMenu) {
				std::cout << "you are in pause menu!\tmain_state: " << main_state << " gamemode: " << gamemode << std::endl;
			}
			else {
				ui.mainMenu();
				if (ui.gameStateTest != ui.mainmenu()) main_state = main_state_switch();
				//std::cout << "you are in main menu!\tmain_state: " << main_state << " gamemode: " << gamemode << std::endl;
				timeBuffer = glfwGetTime();
			}
			break;
		case 1: //runs render and sends the gamemode variable
			//main_state = renderHandler->render(gamemode); //gamemode has been decided in case 0/menu
			time = glfwGetTime() - timeBuffer;
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			renderHandler->renderer();
			ui.inGameStats(1, 2, 7.0f - time);
			
			if (!checkTimeUp(time));
			else renderHandler->descend();
			break;
		case 2:
			std::cout << "game will now exit!\tmain_state: " << main_state << " gamemode: " << gamemode << std::endl;

			break;

		default:
			std::cout << "something very weird happened! This should absofruitely not happen!" << std::endl;
		}
		
		input();
		glfwPollEvents();
		glfwSwapBuffers(gameWindow->winWindow);

	}
	ui.closeImgui();
	glfwTerminate();
}

int game::main_state_switch(){
	if (ui.gameStateTest == ui.rungame()) { return 1; }
	else return 0;
}

void game::input()
{
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gameWindow->winWindow, true);
		
	}
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_UP) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(0, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_DOWN) == GLFW_PRESS && !buttonPressed) {

		buttonPressed = true;
		renderHandler->keyInput(1, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_LEFT) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(2, cameraSpeed);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_RIGHT) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(3, cameraSpeed);
	}

	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(gameWindow->winWindow, GLFW_KEY_DOWN) != GLFW_PRESS &&
		glfwGetKey(gameWindow->winWindow, GLFW_KEY_LEFT) != GLFW_PRESS && glfwGetKey(gameWindow->winWindow, GLFW_KEY_RIGHT) != GLFW_PRESS 
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_SPACE) == !GLFW_PRESS && glfwGetKey(gameWindow->winWindow, GLFW_KEY_Q) != GLFW_PRESS 
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(gameWindow->winWindow, GLFW_KEY_W) != GLFW_PRESS && 
		glfwGetKey(gameWindow->winWindow, GLFW_KEY_S) != GLFW_PRESS && glfwGetKey(gameWindow->winWindow, GLFW_KEY_E) == !GLFW_PRESS
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_D) == !GLFW_PRESS)
		buttonPressed = false;

	//yaw
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_Q) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(4, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_A) == GLFW_PRESS && !buttonPressed) {

		buttonPressed = true;
		renderHandler->keyInput(5, cameraSpeed);
	}
	//pitch
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_W) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(6, cameraSpeed);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_S) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(7, cameraSpeed);
	}
	//roll
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_E) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(8, cameraSpeed);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_D) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(9, cameraSpeed);
	}


	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(10, cameraSpeed);
		timeBuffer += time - 7.0f;
	}

	glfwGetCursorPos(gameWindow->winWindow, &xPos, &yPos);

	glm::vec3 direction;
	direction.x = cos(glm::radians(-270.0f)) * cos(glm::radians(0.0f));
	direction.y = sin(glm::radians(0.0f));
	direction.z = sin(glm::radians(-270.0f)) * cos(glm::radians(0.0f));
	renderHandler->mouseInput(direction);
	lastxoffset = 0.0f; lastyoffset = 0.0f;
	//std::cout << "pitch: " << pitch << " yaw: " << yaw << std::endl;
}

bool game::checkTimeUp(float time) {
	if (time >= 7.0f) { timeBuffer += 7.0f; return true; }
	else return false;;
}

void game::mouseInput()
{
	
}

