# PBR-Renderer
Sinple PBR renderer for learning purposes. 

[Sample renders]

## Features
* Physically based Rendering with Cook-Torrance BRDF
* Normal Mapping
## Build instructions
If you want to simply build and test the project type the next command and that should be it!
``` bash
make
./bin/pbr
```
If you want to modify the code, if no more source files are created the before method would work fine. If not, project files need to be recreated, and for that the tool [premake](https://premake.github.io/) is used, specifically the fifth version. To recreate project files and rebuild 
```bash
premake5 gmake2
make clean && make
./bin/pbr
```
## Known issues
* Nvidia users, im sorry! For reasons I don't really know, GLEW (the OpenGL loader) does not find any OpenGL version on the system. 
