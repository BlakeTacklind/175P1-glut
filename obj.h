
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
  obj(unsigned int numPoints, pnt *points);
  virtual ~obj();
  //obj(const obj& orig);
  
  /*
   * getters
   */
  static obj getObject(int i){return objectList[i];};
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
  
  class line{
    
  public:
    line(pnt a, pnt b);
    
  private:
    void Fill(bool BAmode);
    
    pnt p1;
    pnt p2;
    
    int dx;
    int dy;
    
    bool xTravel;
    
    pnt* fill;
  };
  
private:
  //Gets the centroid point of an object
  pnt getCentroid();
  
  static unsigned int nObjects;
  static obj* objectList;
  static char* storedFileName;
    
  pnt* pointList;
  unsigned int nPoints; 
};

#endif
