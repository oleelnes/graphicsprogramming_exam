#include "ImguiHandler.h"
#include <iostream>
#include <sstream>

void ImguiHandler::inGameStats(int score, int layer, float time){
	ImGuiWindowFlags window_flags = 0;					
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoInputs;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoDecoration;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.9f, io.DisplaySize.y * 0.4f), 
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	std::string timeBuffer = "Descension in: " + std::to_string(time);
	std::string scoreBuffer = "\nScore: " + std::to_string(score);		
	std::string layerBuffer = "\nLayer: " + std::to_string(layer);			
	// display data
	ImGui::Begin("in-game stats", NULL, window_flags);
	ImGui::TextColored(colorBlue, timeBuffer.c_str());
	ImGui::TextColored(colorBlue, scoreBuffer.c_str());
	ImGui::TextColored(colorBlue, layerBuffer.c_str());
	//ImGui::PushFont()
	ImGui::SetWindowFontScale(2.0f);
	ImGui::End();
	// Display
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiHandler::mainMenu(){
	ImGuiWindowFlags window_flags = 0;						
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoDecoration;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	
	ImGui::NewFrame();
	// menu at the center of screen setNextWindowPos -> sets position of next window of imgui::begin

	//ImGui::SetNextWindowSize(ImVec2(200, 200.0f), ImGuiCond_Always); //sets imgui window size to something (in pixels)
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), \
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::Begin("Main Menu", NULL, window_flags);
	//ImGui::PushStyleColor(ImGuiCol_Text, colorBlue);
	ImGui::Text("Main Menu");
	//ImGui::Button("Start game", ImVec2(200.0f, 20.0f)); //sets size of button in pixels
	if (ImGui::Button("Play BLOCK OUT") || detectEnterPress()) {				// Creates button. If clicked sets bools val
		gameStateTest = gameState::RUNGAME;
	}
	if (ImGui::Button("Exit") || detectEnterPress()) {	// Creates button. If clicked sets bools val
		gameStateTest = gameState::ENDGAME;
	}
	//ImGui::PopStyleColor();
	ImGui::SetWindowFontScale(1.25f);
	ImGui::End();
	// Display
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiHandler::closeImgui(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool ImguiHandler::detectEnterPress()
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly) && ImGui::IsKeyPressed(GLFW_KEY_ENTER)) return true;
	else return false;
}


