

#include "OpenGLhandler.h"
#include "object2D.h"
#include "object3D.h"
#include "object3Dsurface.h"
#include "userInterface.h"
#include "screen.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //initialize Values
  //OpenGLhandler::initValues(argc, argv);
 
  //start UI
  userInterface::init();
  
  //load objects from default file
  object3Dsurface::load("objectSurface");
  
  //draw objects
  //screen::bufferAllScreens();
  
  //initialize glut!
  //OpenGLhandler::init(&argc, argv);

  //char c;
  //while((c = getchar()) != 27) userInterface::keypressed(c);
  
  //OpenGLhandler::onClose();
  return 0;
}

