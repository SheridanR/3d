# 3d
a simple 3d software renderer in < 1000 lines of C.

![picture of a 3d object](https://raw.githubusercontent.com/SheridanR/3d/master/screenshot.png)

## Compiling
You need will SDL2 to build the project. (https://www.libsdl.org/download-2.0.php)
If you want to, you can also easily port to a different window/input library.
All of the SDL2-specific code is in crap.c / crap.h.

### Windows
Place the headers/libs for SDL2 under a folder named `external`, then run build.bat from the Visual Studio command prompt.

### macOS
Install the latest version of xcode and the SDL2 Framework, then run build.sh.

## Running
Drag a wavefront .obj file (3D model) onto the executable from your file manager, or specify a path to it as an argument.
By default, the program tries to load the file from `mesh/cube.obj`
You can fly around and inspect the model with WASDQE and the numpad keys.

## Contact
Reach out to sheridan.rathbun@gmail.com with comments, suggestions, etc.