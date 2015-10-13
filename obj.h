
#ifndef OBJ_H
#define	OBJ_H

#include <string>
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
  obj(const obj& orig);
  

  //Get Clipped object
  static void clipObjects(int xmin, int xmax, int ymin, int ymax);
  obj clip(int xmin, int xmax, int ymin, int ymax);

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
 /* 
  class line{
    
  public:
    line(pnt a, pnt b, bool BAmode);
    virtual ~line();
    line(const line &orig);
    //line& operator= (const line& other);

    inline unsigned int getNumPoints(){return xTravel?dx:dy;};
    inline pnt getPoint(unsigned int i){return fill[i];};
    inline bool isHorizontal(){return !dy;};
    
    inline pnt getP1(){return p1;};
    inline pnt getP2(){return p2;};
    
  private:
    void Fill(bool BAmode);
    
    pnt p1;
    pnt p2;
    
    int dx;
    int dy;
    
    bool xTravel;
    
    pnt* fill;
  };
  */
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
};

#endif
