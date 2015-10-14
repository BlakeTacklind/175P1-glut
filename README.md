# ECS175 - Project 1
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
Possible to have 2 arguments for width and height\
example, width=300, height=200:
```
a.out 300 200
```
#### Exit
When the window is open and in focus and no operations are being done. hit [ESC]\
Not doing so may cause memory leaks

### Using UI
**IMPORTANT:** Input location changes depending on current action\
To see which window should have focus - see second line of UI\
Hit [h] key for help and list of avaible actions\
Hitting specific keys while GLUT window is the focus and currently in not in a value entering mode.\
Values are entered as integers or floats depending on the situation.\
If two values are required (translation or scalling) hit enter between each\
Changing viewport is a little different then the other modes. Hit an arrow key to choose which view port side to change.

### Features
* Display objects
* Load object from file
* Save objects to file
* Transform objects relative to centroid
  * Translation
  * Scaling
  * Rotation
* Clippping objects inside a rectangular viewport
  * Sutherland-Hodgman Algorithm
* Draw clipped objects
  * Vertexs
  * Contour Lines
  * Rasterization
* Line drawing algorithm
  * Digital differential analyzer
  * Bresenham's

### Input File
1. First line is a single integer number for number of objects. Nothing else
2. Next Line: Define number of points for object as integer. Nothing else
3. Next Line: Define point with 2 integers delimited by a single space. Nothing else
4. Repeat step 3 for each point in object. Exactly
5. Repeat step 2,3,4 for each object.

Note: same as example input file but without comments

### Defaults
* Origin: bottom left corner (0,0)
* screen size: 200x200
* load file: 'objects'
* draw mode: contour lines
* line drawing algorithm: DDA

##### Known Issues
* Only way add an object or points to an object is to add it to the input file
* Ocasional rasteriztion parity failure on complex objects
* When clipping lines. The approximation of point along edge may cause small changes in outer line
* Does not handle a bad input file gracefully

