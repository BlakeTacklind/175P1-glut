# ECS175 - Project 4
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
Hitting specific keys while GLUT window is the focus will change modes\
Hit [h] key for help and list of avaible actions\
When in modify mode, click on a control point to edit it's position.\
Or click and hold and drag to move the point.\
When in add mode click on a line to add a point between those two points or before or after to append a new point.\
Hold and drag to place the point where ever you want.\
When in remove mode click on a point to remove it.\
To edit knots: select a b-Spline curve (click near it) and a series of lines apear near the top.\
These represent knot positions, click before a knot to modify that one.\
Hold to drag it, click to enter specific value.\
When entering specific value for new variables, hit [tab], [space], [enter] to go to the next value.\
Hit [backspace] to revert a letter. It is possible to clear the entire value this way.\
Hitting [esc] while entering a value will cancel the change.\
Hitting [esc] while NOT entering a value will end the program.\

### Features
* Displays 2D curves.
  * Possible to use Bezier (located: "curve2d.cpp" line: 208-234)
  * Possible to use B-Spline (located: "curve2d.cpp" line: 236-280)
* Scales image so each scene is entirely displayed
* Load objects from file
* Saves objects to file
* Set Resolution for curves (number of points calculated)
* Will find maximum resolution needed to display curves 'exactly' (hit 'e')

### Input File
1. First line is a single integer number for number of curve. Nothing else
2. Next Line: Indicate if following curve definition is bezier or b-spline with "bz", or "bs" respectively. Nothing Else.
3. If B-Spline: Next Line: define k. single positive integer. Nothing else
4. Next Line: Define number of control points for curve as integer. Nothing else
5. Next Line: Define point with 2 floats delimited by a single space. Nothing else
6. Repeat step 5 for each point in curve. Exactly
7. If B-Spline: Next Line: "T" of "F" for if knot positions defined or not.
  * If "T": Next Line: knot position of u[i]. Nothing else
    * repeat for each knot (k + numPoints - 1 times)
8. Reapeat Step 2-7 for each curve

### Defaults
* screen size: 800x800
* load file: 'curveObjects'
* bezier curves: red
* b-Spline: green
* control lines: white
* Default knot distance is 1

##### Known Issues
* Occasional crashes when modifying knots. Especially combining knots.
* Rare crash related to removing point from b-Spline
