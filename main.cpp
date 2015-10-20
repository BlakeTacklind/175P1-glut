#include "OpenGLhandler.h"
#include "object2D.h"
#include "userInterface.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //initilize Values
  OpenGLhandler::initValues(argc, argv);
 
  //start UI
  userInterface::init();
  
  //load objects from default file
  object2D::load("objects");
  
  //draw objects
  OpenGLhandler::bufferObjects();
  
  //initilize glut!
  OpenGLhandler::init(&argc, argv);
  return 0;
}

