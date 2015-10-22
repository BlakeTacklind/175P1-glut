
#ifndef TYPES_H
#define	TYPES_H


/*
 * Struct for 2D points (integer)
 */
typedef struct p2{
  int x, y;
} pnt;

/*
 * Struct for 2D points (floats)
 */
typedef struct pf2{
  float x, y;
} pntf;

/*
 * Struct for 3D points (floats)
 */
typedef struct pf3{
  float x, y, z;
} pnt3;

/*
 * Struct for point edge combinations
 */
typedef struct e{
  int p1, p2;
} edge;

const pnt3 zeroVector = {0, 0, 0};

const pnt3 unitX = {1, 0, 0};
const pnt3 unitY = {0, 1, 0};
const pnt3 unitZ = {0, 0, 1};

bool operator==(const pnt3& a, const pnt3& b);
float operator*(const pnt3& a, const pnt3& b);
pnt3 operator%(const pnt3& a, const pnt3& b);
pnt3 operator*(const float i, const pnt3& p);
pnt3 operator*(const pnt3& p, const float i);
pnt3 operator/(const pnt3& p, const float i);
pnt3 operator~(const pnt3& p);
pnt3 operator-(const pnt3& a);
pnt3 operator-(const pnt3& a, const pnt3& b);
pnt3 operator+(const pnt3& a, const pnt3& b);
pnt3 operator<<(const pnt3& a, const pnt3& b);
pnt3 rotateAboutZ(const pnt3& p, const float a);

const float deg2rad = 3.14159265/180;

#endif	/* TYPES_H */

