#include "window.h"

window::window(){
	//empty!! make it so the title of the window is editable through the making of the pointer.,!
}

void window::windowCreator(){
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);
	winWindow = glfwCreateWindow(1920, 1080, "Block", glfwGetPrimaryMonitor(), NULL);
	glfwSetWindowAspectRatio(winWindow, 16, 9);

	glfwMakeContextCurrent(winWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
