#include "types.h"
#include <math.h>

//char* toString(pnt3 p){}

/* check for vector equality */
bool operator==(const pnt& a, const pnt& b){
  return a.x == b.x && a.y == b.y;
}

/* check for vector equality */
bool operator==(const pnt3& a, const pnt3& b){
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

/* dot product of vectors */
float operator*(const pnt3& a, const pnt3& b){
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* vector multiplied by a scaler */
pnt3 operator*(const pnt3& p, const float i){
  if (i == 1) return p;

  pnt3 o = {p.x*i, p.y*i, p.z*i};
  return o;
}

/* vector multiplied by a scaler */
pnt3 operator*(const float i, const pnt3& p){
  if (i == 1) return p;

  pnt3 o = {p.x*i, p.y*i, p.z*i};
  return o;
}

/* vector divided by a scaler */
pnt3 operator/(const pnt3& p, const float i){
  if (i == 1) return p;

  pnt3 o = {p.x/i, p.y/i, p.z/i};
  return o;
}

/* Normalize vector operator */
pnt3 operator~(const pnt3& p){
  float d = sqrt(p*p);

  if(d == 1) return p;
  else       return p/d;
}

/* negate vector */
pnt3 operator-(const pnt3& a){
  if(a==zeroVector) return zeroVector;
  pnt3 p = {-a.x, -a.y, -a.z};
  return p;
}

/* subtracting vectors */
pnt3 operator-(const pnt3& a, const pnt3& b){
  if(b==zeroVector) return  a;
  if(a==zeroVector) return -b;
  pnt3 p = {a.x-b.x, a.y-b.y, a.z-b.z};
  return p;
}

/* subtracting vectors */
pnt3 operator+(const pnt3& a, const pnt3& b){
  if(b==zeroVector) return a;
  if(a==zeroVector) return b;
  pnt3 p = {a.x+b.x, a.y+b.y, a.z+b.z};
  return p;
}

/* Find projection of vector b onto a normal vector a */
pnt3 operator<<(const pnt3& a, const pnt3& b){
  float dot = a*b;
  //if othogonal already return self
  if(dot==0) return b;
  //otherwise find the projection
  return b - (a * dot);
}

/* cross product of vectors */
pnt3 operator%(const pnt3& a, const pnt3& b){
	if(a==unitX && b==unitY) return unitZ;
	if(a==unitY && b==unitZ) return unitX;
	if(a==unitZ && b==unitX) return unitY;

	pnt3 p = {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
	return p;
}

/*
 * rotate point p about the z axis by a degrees
 */
pnt3 rotateAboutZ(const pnt3& p, const float a){
  float rad=a*deg2rad;
  float ca = cos(rad);
  float sa = sin(rad);

  pnt3 o = {p.x*ca - p.y*sa, p.x*sa + p.y*ca, p.z};
  return o;
}

/*
 * get the length of a vector
 */
float length(const pnt3& vec){
  return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

pnt3& flatten(pnt3& p){
  if(p.x > 1) p = p/p.x;
  if(p.y > 1) p = p/p.y;
  if(p.z > 1) p = p/p.z;
  return p;
}
