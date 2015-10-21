

#include "OpenGLhandler.h"
#include "object2D.h"
#include "object3D.h"
#include "userInterface.h"
#include "screen.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //initialize Values
  OpenGLhandler::initValues(argc, argv);
 
  //start UI
  userInterface::init();
  
  //load objects from default file
  object3D::load("objects3d");
  
  //draw objects
  screen::bufferAllScreens();
  
  //initialize glut!
  OpenGLhandler::init(&argc, argv);
  return 0;
}

