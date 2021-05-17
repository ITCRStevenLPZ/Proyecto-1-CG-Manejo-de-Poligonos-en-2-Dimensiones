#include <GL/glut.h>
#define maxHt 800
#define maxWd 600
#define maxVer 10000
  
FILE *fp;
  
// Start from lower left corner
typedef struct edgebucket 
{
    int ymax;   //max y-coordinate of edge
    float xofymin;  //x-coordinate of lowest edge point updated only in aet
    float slopeinverse;
}EdgeBucket;
  
typedef struct edgetabletup
{
    // the array will give the scanline number
    // The edge table (ET) with edges entries sorted 
    // in increasing y and x of the lower end
      
    int countEdgeBucket;    //no. of edgebuckets
    EdgeBucket buckets[maxVer];
}EdgeTableTuple;
  
EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;
  