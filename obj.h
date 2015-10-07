
#include <string>
using namespace std;

class obj{
  typedef struct p{
    int x;
    int y;
  } pnt;
public:
  /*
   * Load and save from file
   */
  inline static void load(){load(storedFileName);};
  inline static void save(){save(storedFileName);};
  static void load(char* filename);
  static void save(char* filename);
    
  /*
   * Default class definitions
   */
  obj(unsigned int numPoints, pnt *points);
  virtual ~obj();
  obj(const obj& orig);
  
  /*
   * getters
   */
  static obj getObject(int i){return objectList[i];};
  inline unsigned int getNumPoints(){return nPoints;};  
  inline pnt* getPoints(){return pointList;};
  
  /*
   * toString items
   * Doesn't work well
   */
  string getString();
  static string getTotalString();

private:
  static unsigned int getNumberFromLine(string line);
  
  static unsigned int nObjects;
  static obj* objectList;
  static char* storedFileName;
    
  pnt* pointList;
  unsigned int nPoints; 
};

