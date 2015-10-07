

class obj{
public:
  static typedef struct{
    int x;
    int y;
  } pnt;
  obj(unsigned int numPoints, pnt* points);
  inline unsigned int getNumPoints(){return nPoints;};  
  inline pnt* getPoints(){return pointList;};
  

private:
  pnt* pointList;
  unsigned int nPoints; 
};

