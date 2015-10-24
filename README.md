# ECS175 - Project 2
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
**IMPORTANT:** Input location changes depending on current action\
To see which window should have focus - see second line of UI\
Hit [h] key for help and list of avaible actions\
Hitting specific keys while GLUT window is the focus and currently in not in a value entering mode.\
Values are entered as integers or floats depending on the situation.\
If two values are required (translation or scalling) hit enter between each\
Changing viewport is a little different then the other modes. Hit an arrow key to choose which view port side to change.

### Features
* Display 3D objects with 4 different angles Axonometric Projections
  * Qudrant 1, 2, 3 display relative to XZ, XY, YZ planes respectively
  * Quadrant 4 may display compared to any vector in R3
* Scales image so each scene is entirely displayed
* Load objects from file
* Save objects to file
* Transform objects
  * Translation
  * Scaling relative to centroid
  * Rotation realtive to specified axis
* Uses Bresenham's Line drawing algorithm

### Input File
1. First line is a single integer number for number of objects. Nothing else
2. Next Line: Define number of points for object as integer. Nothing else
3. Next Line: Define point with 3 floats delimited by a single space. Nothing else
4. Repeat step 3 for each point in object. Exactly
5. Next Line: Define number of edges
6. Next Line: Define edge with 2 integers delimited by a single space. Nothing else
  * points begin at zero index and are enumerated in provided order
7. Repeat step 6 for each edge in object. Exactly
8. Repeat step 2-7 for each object.

Note: same as example input file but without comments

### Defaults
* screen size: 400x400
* load file: 'objects3d'

##### Known Issues
* Only way add an object or points to an object is to add it to the input file
* UI is inefficiant
