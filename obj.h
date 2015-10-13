
#ifndef OBJ_H
#define	OBJ_H

#include <string>
#include <list>
#include "line.h"
#include "types.h"

using namespace std;

class obj{
public:
  /*
   * Load and save from file
   */
  inline static void load(){load(storedFileName);};
  inline static void save(){save(storedFileName);};
  static void load(char* filename);
  static void save(char* filename);

  /*
   * Free memory
   */  
  static void freeAll();

  /*
   * Default class definitions
   */
  inline obj(){nPoints = 0;};
  obj(unsigned int numPoints, pnt *points);
  virtual ~obj();

  //Clipping object functions
  static void clipObjects(int xmin, int xmax, int ymin, int ymax);
  inline static void cleanClippedObects(){delete [] clippedObjects;};

  //Fill the points of the objects
  void fill(void (*MakePix)(int, int), bool BAmode);

  /*
   * Getters
   */
  inline static unsigned int getNumClippedObjects(){return nClippedObjects;};
  inline static obj getClippedObject(int i){return clippedObjects[i];};
  inline static obj getObject(int i){return objectList[i];};
  inline unsigned int getNumPoints(){return nPoints;};  
  inline pnt* getPoints(){return pointList;};
  inline static unsigned int getNumObjects(){return nObjects;};
  
  /*
   * Transform objects
   */
  void translation(int x, int y);
  void rotation(float alpha);
  void scale(float x, float y);
 
private:
  //clip this object
  obj clip(int xmin, int xmax, int ymin, int ymax);
  //Gets the centroid point of an object
  pnt getCentroid();
  void findInList(list<line*> &l, int x, int y, bool* out);
  void shortenList(list<line*> &l, int y);
  
  /*
   * variables
   */
  
  static unsigned int nClippedObjects;
  static obj* clippedObjects;

  static unsigned int nObjects;
  static obj* objectList;

  static char* storedFileName;
    
  pnt* pointList;
  unsigned int nPoints; 
  
  int xMin, xMax, yMin, yMax;
};

#endif
