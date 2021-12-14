BLOCKOUT Game
Exam for prog2002

Version 0.6

Date: 14/12/2021
Time: 22:15

New in version 0.6: 
* Active block now descends one block further down the tunnel if seven seconds passes
	- Every time it descends one block, the necessary collision detection functions are called
	  in order to check whether or not it should solidify.
* Added some more sources


How to run (visual studio): 
	1. Copy repository web link
	2. Open Visual Studio
	3. Select "Clone a repository"
	4. Add repository link to "repository location" and click "clone"
	5. When is finished CMake configuring the project, select "Eksamen.exe" as
	   startup item, then run it
	6. Now, the program should run


Controls:
ESC: Exits game
UP: Up
DOWN: Down
LEFT: Left
RIGHT: Right
SPACE: Release block

NOTE: All textures are self-made.

VERSIONS:
0.1: The general layout of the level has been added. 
0.2: Active block has been added but not completed; lighting improved.
0.3: 2 textures added (self-drawn); collision mechanics under development; minor code improvements
0.4: Collision mechanics fully developed; Fullscreen added; Started writing down sources; 
Texture for inactive blocks added
0.5: Added textures for sections of the tunnel; section-dependent texture/color on solid blocks added; 
added TextureLoader-class.
0.6: Current version (See "new in version 0.6") 


SOURCES:
1. Shader.h and Shader.cpp: 
Accessed 17.11.2021. Available at: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h 
Also avaiable at: https://learnopengl.com/Getting-started/Shaders
(Not an identical copy, but mostly made by strictly following the steps)
2. ImguiHandler.cpp and ImguiHandler.h:
Made through following tutorials.
2.1: Accessed 11.12.2021. Available at: https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
2.2: Accessed 11.12.2021. Available at: https://github.com/ocornut/imgui/tree/master/examples
2.3: Accessed 09.12.2021. Available at: https://www.youtube.com/watch?v=nVaQuNXueFw&ab_channel=TheCherno
More sources to come!