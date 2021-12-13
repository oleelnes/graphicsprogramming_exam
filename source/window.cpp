#include "window.h"

window::window(){
	//empty!! make it so the title of the window is editable through the making of the pointer.,!
}

void window::windowCreator(){
	winWindow = glfwCreateWindow(800, 800, "Exam!!", NULL, NULL);
	glfwSetWindowAspectRatio(winWindow, 28, 28);

	glfwMakeContextCurrent(winWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
