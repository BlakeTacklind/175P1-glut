
#ifndef OBJ_H
#define	OBJ_H

#include <string>
//#include <cstring>

using namespace std;

class obj{
public:
  /*
   * Struct for points
   */
  typedef struct p{
    int x;
    int y;
  } pnt;
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
  //obj(const obj& orig);
  //inline obj& operator= (const obj& other);  

  //Get Clipped object
  static void clipObjects(int xmin, int xmax, int ymin, int ymax);
  obj clip(int xmin, int xmax, int ymin, int ymax);

  void fill(void (*MakePix)(int, int), bool BAmode);

  /*
   * getters
   */
  inline static unsigned int getNumClippedObjects(){return nClippedObjects;};
  inline static obj getClippedObject(int i){return clippedObjects[i];};
  inline static obj getObject(int i){return objectList[i];};
  inline unsigned int getNumPoints(){return nPoints;};  
  inline pnt* getPoints(){return pointList;};
  inline static unsigned int getNumObjects(){return nObjects;};
  
  /*
   * toString items
   * Doesn't work well
   */
  string getString();
  static string getTotalString();

  
  /*
   * Transform objects
   */
  void translation(int x, int y);
  void rotation(float alpha);
  void scale(float x, float y);
 
private:
  //Gets the centroid point of an object
  pnt getCentroid();
  
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
