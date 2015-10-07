
#include <string>
using namespace std;

class obj{
  typedef struct p{
    int x;
    int y;
  } pnt;
public:
  inline static void load(){load(storedFileName);};
  inline static void save(){save(storedFileName);};
  static void load(char* filename);
  static void save(char* filename);
    
  obj(unsigned int numPoints, pnt *points);
  inline unsigned int getNumPoints(){return nPoints;};  
  inline pnt* getPoints(){return pointList;};
  
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

