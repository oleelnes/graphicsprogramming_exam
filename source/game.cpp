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


/*
	This function runs the game. The main while-loop for the OpenGL rendering recides within this function
*/
void game::run(){
	gameInit();
	
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(gameWindow->winWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	
	glEnable(GL_DEPTH_TEST);
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	int gamemode = 0;
	int main_state = 0;
	int score = 0;
	int layer = 0;

	time = 0.0f;
	timeBuffer = 0.0f;

	glClearColor(0.2f, 0.03f, 0.5f, 1.0f);
	while (!glfwWindowShouldClose(gameWindow->winWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		renderHandler->updateSpeed(2.5 * deltaTime);
		
		switch (main_state) {
		case 0: //menu -- decides int gamemode and changes main_state to 1 or 2 depending on whether a gamemode has been selected or if exit game has been selected
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ui.mainMenu();
			if (ui.gameStateTest != ui.mainmenu() && !buttonPressed) { main_state = main_state_switch(); renderHandler = new RenderHandler(); }
			timeBuffer = glfwGetTime();
			break;
		case 1: //runs render and sends the gamemode variable
			time = glfwGetTime() - timeBuffer;
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			renderHandler->renderer();
			score = renderHandler->getScore();
			layer = 11 - renderHandler->getLayer();
			if (!renderHandler->getGameOver()) {
				ui.inGameStats(score, layer, 7.0f - time);
				//checks whether time before descent is up, if it is, the descend() function is called which will trigger a one block descent in the
				//through the renderHandler-pointer
				if (!checkTimeUp(time));
				else renderHandler->descend();
			}
			else {
				ui.gameOverStats(score);
				if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !buttonPressed) {
					main_state = 0;
					buttonPressed = true;
				}
			}
			break;
		default:
			break;
		}
		input();
		glfwPollEvents();
		glfwSwapBuffers(gameWindow->winWindow);
	}
	ui.closeImgui();
	glfwTerminate();
}

//If gamestate matches what ui.rungame() returns, 1 is returned, which means the main menu will be exited and the game will start
int game::main_state_switch(){
	if (ui.gameStateTest == ui.rungame()) { return 1; }
	else return 0;
}

//This function checks for input and sends it to the keyInput-function through the renderHandler pointer
void game::input()
{
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gameWindow->winWindow, true);
		
	}
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_UP) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(0);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_DOWN) == GLFW_PRESS && !buttonPressed) {

		buttonPressed = true;
		renderHandler->keyInput(1);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_LEFT) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(2);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_RIGHT) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(3);
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
		renderHandler->keyInput(4);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_A) == GLFW_PRESS && !buttonPressed) {

		buttonPressed = true;
		renderHandler->keyInput(5);
	}
	//pitch
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_W) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(6);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_S) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(7);
	}
	//roll
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_E) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(8);

	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_D) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(9);
	}


	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_SPACE) == GLFW_PRESS && !buttonPressed) {
		buttonPressed = true;
		renderHandler->keyInput(10);
		timeBuffer += time - 7.0f;
	}

	glfwGetCursorPos(gameWindow->winWindow, &xPos, &yPos);

	glm::vec3 direction;
	direction.x = cos(glm::radians(-270.0f)) * cos(glm::radians(0.0f));
	direction.y = sin(glm::radians(0.0f));
	direction.z = sin(glm::radians(-270.0f)) * cos(glm::radians(0.0f));
	renderHandler->mouseInput(direction);
	lastxoffset = 0.0f; lastyoffset = 0.0f;
}

bool game::checkTimeUp(float time) {
	if (time >= 7.0f) { timeBuffer += 7.0f; return true; }
	else return false;;
}


