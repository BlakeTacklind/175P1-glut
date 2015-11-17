# ECS175 - Project 3
### by Blake Tacklind <bztack@ucdavis.edu>

---
#### How to Build
```
make
````
#### Run
```
a.out
```
#### Exit
When the window is open and in focus and no operations are being done. hit [ESC]\
Not doing so may cause memory leaks

### Using UI
**IMPORTANT:** Keep Glut Window in focus\
Hit [h] key for help and list of avaible actions\
Hitting specific keys while GLUT window is the focus will begin value altering mode\
Note: Must not already be in value altering mode.\
Enter value for new variables, hit [tab], [space], [enter] to go to the next value.\
Hit [backspace] to revert a letter. It is possible to clear the entire value this way.\
Hitting [esc] while entering a value will cancel the change.\
Hitting [esc] while NOT entering a value will end the program.\

### Features
* Display 3D objects with surfaces in 4 different angles Axonometric Projections
  * Qudrant 1, 2, 3 display relative to XZ, XY, YZ planes respectively
  * Quadrant 4 may display compared to any vector in R3
* Scales image so each scene is entirely displayed
* Load objects from file
* displays objects with lighting
  * Uses Phong Model to find light at vertexes (located: "getColorFunc.h")
  * Uses Gouraud shading to shade edges and surfaces (located: "screen.cpp", line 228-238 & "gline.cpp" line 27-38)
* Removes hidden surfaces with Painter's Algorithm. (located: "screen.cpp", line 90-97)
  * Possible to see by changing the draw mode to lines. [r] key x2
* Sorts lines with Painter's Algorithm (located: "screen.cpp", line 116 & "screen.cpp" line 70-72)
* Can continuously rotate quadrant 4 displayed image by **holding Space key**

### Input File
1. First line is a single integer number for number of objects. Nothing else
2. Next Line: Define number of points for object as integer. Nothing else
3. Next Line: Define point with 3 floats delimited by a single space. Nothing else
4. Repeat step 3 for each point in object. Exactly
5. Next Line: Define number of edges
6. Next Line: Define edge with 2 integers delimited by a single space. Nothing else
  * points begin at zero index and are enumerated in provided order
7. Repeat step 6 for each edge in object. Exactly
8. Next Line: number of surfaces
9. Next Line: index of points that define a surface delimited by a single space. 
  * At least three points
10. Next Line: 3 floating point numbers that is inline with the outward facing normal. single space delimited. Nothing else.
11. reapeat step 9-10 for each surface. Exactly
12. Repeat step 2-11 for each object.

### Defaults
* screen size: 402x402
* load file: 'objectsSurface'
* Ambiant: Red
* Diffuse: Green
* Specular: Blue
* light position (1,1,3)
* Further defaults can be seen by loading program.
* MegaPixels are 3x3 squares of pixels with points randomly switched around.
* quadrant 4 normal vector: oblique <0.612375, 0.612375, -0.50000>

##### Known Issues
* Only way add an object or points to an object is to add it to the input file
* Does not display light properly with the light point being on a vertex
* Requires Acurate normals for surfaces
* objects are expected to be closed by surfaces
* does not handle intersecting objects
