

#include "OpenGLhandler.h"
//#include "object2D.h"
//#include "object3D.h"
//#include "object3Dsurface.h"
#include "curve2d.h"
#include "userInterface.h"
#include "screen2d.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //initialize Values
  OpenGLhandler::initValues(argc, argv);
 
  //load objects from default file
  curve2d::load("curveObjects");
  //start UI
  //userInterface::init();

  //draw objects
  screen2d::drawAll();
  
  //initialize glut!
  OpenGLhandler::init(&argc, argv);

  //char c;
  //while((c = getchar()) != 27) userInterface::keypressed(c);
  
  OpenGLhandler::onClose();
  return 0;
}

