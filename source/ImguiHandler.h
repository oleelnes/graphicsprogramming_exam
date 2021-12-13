#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include "imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "window.h"



enum class gameState {MAINMENU, RUNGAME, PAUSEMENU, GAMEOVERMENU, ENDGAME};
class ImguiHandler{	
private:
	ImVec4 colorYellow = ImVec4(1, 1, 0, 1);	// Yellow text
	ImVec4 colorBlue = ImVec4(0, 0, 1, 1);		// Blue text
public:
	gameState gameStateTest; // = testing::gameState::MENU;
	ImguiHandler() { 
		gameStateTest = gameState::MAINMENU; 
	}
	gameState rungame() { return gameState::RUNGAME; } //change to style gs_rungame, put "gs_" in front of all of these
	gameState mainmenu() { return gameState::MAINMENU; }
	gameState pausemenu() { return gameState::PAUSEMENU; }
	gameState gameovermenu() { return gameState::GAMEOVERMENU; }
	gameState endgame() { return gameState::ENDGAME; }
	void inGameStats(int score, int health);
	void mainMenu();
	void closeImgui();
	bool detectEnterPress();
	
};