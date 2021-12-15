#include "ImguiHandler.h"
#include <iostream>
#include <sstream>

/*
	Source 2 (See readme)
*/
void ImguiHandler::inGameStats(int score, int layer, float time){
	ImGuiWindowFlags window_flags = 0;					
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoInputs;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoDecoration;
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.81f, io.DisplaySize.y * 0.3f),
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	std::string timeBuffer = "TIME UNTIL NEXT BLOCK DESCENT: \n" + std::to_string(time);
	std::string scoreBuffer = "\nSCORE: " + std::to_string(score);
	std::string layerBuffer = "\nLAYER: " + std::to_string(layer);	
	ImGui::Begin("in-game stats", NULL, window_flags);
	ImGui::TextColored(colorBlue, timeBuffer.c_str());
	ImGui::TextColored(colorBlue, scoreBuffer.c_str());
	ImGui::TextColored(colorBlue, layerBuffer.c_str());
	ImGui::SetWindowFontScale(2.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
	Source 2 (See readme)
*/
void ImguiHandler::mainMenu(){
	ImGuiWindowFlags window_flags = 0;						
	window_flags |= ImGuiWindowFlags_NoDecoration;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), \
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Main Menu", NULL, window_flags);
	ImGui::Text("MAIN MENU");
	if (ImGui::Button("PLAY BLOCK OUT") || detectEnterPress()) gameStateTest = gameState::RUNGAME;
	if (ImGui::Button("EXIT") || detectEnterPress()) gameStateTest = gameState::ENDGAME;
	ImGui::SetWindowFontScale(1.75f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
	Function that properly shuts down ImGui
*/
void ImguiHandler::closeImgui(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/*
	Boolean function that returns true if enter hovers over a button AND the ENTER key is pressed
*/
bool ImguiHandler::detectEnterPress()
{
	if (ImGui::IsKeyPressed(GLFW_KEY_ENTER) && ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly)) return true;
	else return false;
}


