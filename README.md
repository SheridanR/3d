# 3d
a simple 3d software renderer in < 1000 lines of C.

## compiling
you need SDL2 to build the project. (https://www.libsdl.org/download-2.0.php)
if you want to, you can port to a different window / input library.
all of the SDL2-specific code is in crap.c / crap.h.
check build.bat for an extremely basic build script on windows.

## running
drag a wavefront .obj file (3D model) onto the executable from your file manager, or specify a path to it as an argument.
by default, the program tries to load the file from `mesh/cube.obj`
you can fly around and inspect the model with WASDQE and the numpad keys.

## contact
reach out to sheridan.rathbun@gmail.com with comments, suggestions, etc.