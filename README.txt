BLOCKOUT Game
Exam for prog2002

Version 0.8

Date: 15/12/2021
Time: 19:13

New in version 0.8: 
* Complex active block collision detection with walls added
* Complex blocks will retain their complex form and position when they solidify
* Roll rotation is added and fully functional




How to run (visual studio): 
1. Copy repository web link
2. Open Visual Studio
3. Select "Clone a repository"
4. Add repository link to "repository location" and click "clone"
5. When is finished CMake configuring the project, select "Eksamen.exe" as
   startup item, then run it
6. Now, the program should run



CONTROLS:
ESC: Exits game
UP: Up
DOWN: Down
LEFT: Left
RIGHT: Right
SPACE: Release block
Q: ROLL +1
A: ROLL -1



NOTE: All textures are self-made.



VERSIONS:
0.1: The general layout of the level has been added. 
0.2: Active block has been added but not completed; lighting improved.
0.3: 2 textures added (self-drawn); collision mechanics under development; minor code improvements
0.4: Collision mechanics fully developed; Fullscreen added; Started writing down sources; 
Texture for inactive blocks added
0.5: Added textures for sections of the tunnel; section-dependent texture/color on solid blocks added; 
added TextureLoader-class.
0.6: Active block descends one block depending on time.
0.7: Complex blocks added but without tunnel wall collision detection or in solid form
0.8: Current version (See "new in version 0.8") 



ABOUT SOURCES:
References to sources are in the code, either:
	1. At the top of a class (in the class' .cpp file)
	2. Above a given function 



SOURCE LIST:
1. Shader.h and Shader.cpp: 
Type: class
Utilization of source: Almost an identical copy, mostly made by strictly following the steps of the tutorial.
Accessed 17.11.2021. Available at: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h 
Also avaiable at: https://learnopengl.com/Getting-started/Shaders

2. ImguiHandler.cpp and ImguiHandler.h:
Utilization of source: Tutorial material for the building of the class. The class is not a copy.
2.1: Accessed 11.12.2021. Available at: https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
2.2: Accessed 11.12.2021. Available at: https://github.com/ocornut/imgui/tree/master/examples
2.3: Accessed 09.12.2021. Available at: https://www.youtube.com/watch?v=nVaQuNXueFw&ab_channel=TheCherno

3: TextureLoader.cpp -> load_opengl_texture-function:
Utilization of source: Almost an identical copy of original, but with slight adjustments in order
to fit my code better.
Source: prog2002 course materials/labs -> lab04 -> GLuint load_opengl_texture(const std::string& filepath, GLuint slot)

Source list is NOT finished yet.