
#ifndef OBJ_H
#define	OBJ_H

#include <string>
#include <list>
#include "line.h"
#include "types.h"

using namespace std;

class object2D{
public:
  /*
   * Load and save from file
   */
  inline static bool load(){return load(storedFileName);};
  inline static bool save(){return save(storedFileName);};
  static bool load(char* filename);
  static bool save(char* filename);

  /*
   * Free memory
   */  
  static void freeAll();

  /*
   * Default class definitions
   */
  inline object2D(){nPoints = 0;};
  object2D(unsigned int numPoints, pnt *points);
  virtual ~object2D();

  //Clipping object functions
  static void clipObjects(int xmin, int xmax, int ymin, int ymax);
  inline static void cleanClippedObects(){clippedObjects.clear();};

  //Fill the points of the objects
  void fill(void (*MakePix)(int, int), bool BAmode);

  /*
   * Getters
   */
  typedef list<object2D*>::iterator oITR;
  inline static unsigned int getNumClippedObjects(){return clippedObjects.size();};
  //inline static obj* getClippedObject(int i){return &(clippedObjects[i]);};
  inline static list<object2D*>& getClippedList(){return clippedObjects;};
  inline static object2D* getObject(int i){return objectList[i];};
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
  object2D* clip(int xmin, int xmax, int ymin, int ymax);
  //Gets the centroid point of an object
  pnt getCentroid();
  void findInList(list<line*> &l, int x, int y, bool* out);
  void shortenList(list<line*> &l, int y);
  
  /*
   * variables
   */
  
  static list<object2D*> clippedObjects;

  static unsigned int nObjects;
  static object2D** objectList;

  static char* storedFileName;
    
  pnt* pointList;
  unsigned int nPoints; 
  
  int xMin, xMax, yMin, yMax;
};

#endif
