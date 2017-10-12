# UPC_3DEngine

UPC_3DEngine is a project for the subject 3DEngine of the VideoGame Developing career in CITM, UPC fundation in Terrassa.
The main objective is to create a simple 3D game editor.

## INSTALLATION

Unzip the realese folder and execute the .exe file.


## USAGE

### CONTROLS

-Camera controls
	-Right click turns on “WASD” fps-like movement
	-Mouse wheel zooms in and out
	-Alt+Left click to orbit the object
	-F focus the camera around the geometry
-Drag any FBX and drop it into the executable to load it
-Drag any texture and drop it into the executable to load it

### WINDOWS

-Profiler: This window shows the ms wasted in every module
-Properties: This window shows useful informattion about the 	loaded geometry
-Console: A console that LOGS all the engine process
-Configuration:
	-Application: This window shows useful information of the 		app, FPS, memory usage, etc... It also allows the 		user to cap FPS, change app name, etc...
	-Window: This window shows different window information 		and allows the user to modify some window variables
	-Hardware: This window shows useful hardware information
-Module Variables:

## CHANGELOG

v0.5(First Assignment):

	-

v0.4:

	-Camera autocentre + autoframe to loaded geometry
	-DevIL library added to load textures
	-FBX load their own textures from Assets folder
	-New stryle for the GUI(WIP, may will change)
	-Camera log "hit" if looking to the loaded geometry
	-Cube primitive texure coords added
	-Camera controls changed to be Unity-like

v0.3:

	-Profiler window
	-Module variables window
	-Primitive cube and sphere
	-Drag & drop
	-Assimp implemented
	-Load fbx files
	-Show vertices normals
	-New more clean layout

v0.2:

	-Added Console
	-OpenGl totally operative
	-Added geometry options: Fog, Wireframe, Lightning, 			Culling, etc...
	-All modules frame, ms, memory usage plot
	-First triangles printed on screen
	-Random generator feature added
	-Help and close menu added
	-Hardware data
	-Now you can modify variables from the editor
	-Parson save & load

v0.1:

	-Added base code.
	-Integrated ImGui library.
	-Added menu on top of screen.
	-Added menu that shows example window.
	
## CREDITS

Github project page: https://github.com/xavierolivenza/UPC_3DEngine

Sergio Alvarez's Github account

 - Github: https://github.com/Sergio111


Xavier Olivenza's Github account

 - Github: https://github.com/xavierolivenza


CITM Terrassa 2016-2017
