# GameEngine
##### Semester portfolio, final solo project, and final team project for 3D Game Engine Programming

The main goal of this class was to learn basic concepts of graphics and game development using C++, OpenGL, and in some cases the
Win32 API. We started off with a very simplistic program which we built on over the course of the semester to create a basic yet
fully functional game engine, which included hierarchical objects, simple animations, lighting, basic collision detection, and 2D
texturing. During the development of the engine, we used good software engineering practices and design principles such as continuous 
refactoring, dependency injection, abstraction, and inheritance. By the end of the semester, many of the configuration elements of the 
engine had been moved outside the program to configuration files. For the last couple solo projects, I also created an OBJ file loader,
which enabled me to use Blender to model more complex objects and import them into my game. 

The **Portfolio** is an aggregation of all the individual labs and projects completed over the course of the semester. For each entry,
there is a one page write-up outlining the objectives and important details of the lab/project. Following the write-up are screenshots 
of important algorithms, data structures, and the application running.

**GameEngine** is my final solo project for the semester and is the culmination of all of the previous projects and labs. The "world" 
consists of two rooms attached by a hallway, with each room having some furniture, lights, and animated, hierarchical objects. A fist
person camera represents the player, who can walk up to a light to turn it on/off and turn all the lights in a room on/off by either 
entering or leaving the room. 

**Infinite Runner** is the final project of this class and it was a team effort. My team opted to make an infinite racer game, in which 
the player must dodge obstacles by moving left or right as they continuously move forward. Our game is set in outer space, therefore 
the player controls a space ship and the objects that he/she must avoid are various types of space debris (including an astronaut glove). 
There is also one artificially intelligent enemy who shoots at the player if they get too close. The score increases as a function of 
the time the player has been alive and the health depletes by five if the player hits an obstacle and by one if the player gets shot by 
the enemy. The parts that I worked on in the game are: the skybox, the camera, the publisher-subscriber system used to broadcast events,
the score and health HUDs, and the game asset loader used to load and create shaders, objects, textures, etc. based on the information
stored in an XML data file.
