#ifndef MATH_FRUSTRUM_H
#define MATH_FRUSTRUM_H


#include "math/vector.h"
#include "math/plane.h"
#include "math/bounding_box.h"
#include "math/bounding_sphere.h"

namespace wage {

  class Frustum {

  public:

    enum {
      PLANE_RIGHT = 0,
      PLANE_LEFT,
      PLANE_BOTTOM,
      PLANE_TOP,
      PLANE_FAR,
      PLANE_NEAR      
    };

    Frustum() {}
    
    ~Frustum() {}

    inline int vectorToIndex( const Vector &v ) const {
      int idx = 0;
      if( v.z >= 0 ) idx |= 1;
      if( v.y >= 0 ) idx |= 2;
      if( v.x >= 0 ) idx |= 4;
      return idx;
    }

    int halfPlaneTest(const Vector &p, const Plane &plane) const {
      float dist = plane.distance(p);
      // printf("Dist: %f\n", dist);
      if( dist > 0.02 ) // Point is in front of plane
          return 1;
      else if( dist < -0.02 ) { // Point is behind plane
        // printf("Disatance: %f\n", dist);
        return 0;
      }
      return 2; // Point is on plane
    }

    inline bool contains(Vector point) {
      for (int i = 0; i < 6;i++){
        if (halfPlaneTest( point, planes[i]) == 0) {            
            return false;
        }
      }
      return true;
    }

    inline bool contains(BoundingSphere sphere) {
      for(int i = 0; i < 6; i++) {
        if (planes[i].distance(sphere.position) <= -sphere.radius) {
          return false;
        }
      }
      return true;
    }

    // NOTE: Broken.  Does not handle the case where a box covers entire frustum
    inline bool contains(BoundingBox box) {
      static const Vector cornerOffsets[] = {
        Vector(-1.f,-1.f,-1.f),
        Vector(-1.f,-1.f, 1.f),
        Vector(-1.f, 1.f,-1.f),
        Vector(-1.f, 1.f, 1.f),
        Vector(1.f,-1.f,-1.f),
        Vector(1.f,-1.f, 1.f),
        Vector(1.f, 1.f,-1.f),
        Vector(1.f, 1.f, 1.f) 
      };
      bool boxIn = false;
      for (int j = 0; j < 8; j++) {
        Vector testPoint = box.position + box.halfDim * cornerOffsets[j];  
        printf("TestP: %f:%f:%f\n", testPoint.x, testPoint.y, testPoint.z);
        bool pointIn = true;
        for(int i = 0; i < 6; i++) {
          Vector planeNormal = planes[i].normal;
          if (halfPlaneTest(testPoint, planes[i]) == 0) {
            // printf("TestP: %f:%f:%f\n", testPoint.x, testPoint.y, testPoint.z);
            // printf("BoxP: %f:%f:%f:%f\n", box.position.x, box.position.y, box.position.z, box.halfDim);
            // planes[i].debug();
            // printf("Not good--> %d: %f\n", i, planes[i].d);
            pointIn = false;
            break;  
          }
        }
        printf("PIn: %d\n", pointIn);
        boxIn |= pointIn;
      }
      printf("BIn: %d\n", boxIn);
      return boxIn;
    }

    void extractPlanesFrom(Matrix matrix) {
      // Right Frustum Plane
      // Add first column of the matrix to the fourth column
      planes[PLANE_RIGHT].normal = Vector(
        matrix[0][3] - matrix[0][0],
        matrix[1][3] - matrix[1][0],
        matrix[2][3] - matrix[2][0]
      );
      planes[PLANE_RIGHT].d = matrix[3][3] - matrix[3][0];

      // Left Frustum Plane
      // Subtract first column of matrix from the fourth column
      planes[PLANE_LEFT].normal = Vector(
        matrix[0][3] + matrix[0][0],
        matrix[1][3] + matrix[1][0],
        matrix[2][3] + matrix[2][0]  
      );
      planes[PLANE_LEFT].d = matrix[3][3] + matrix[3][0];

      // Bottom Frustum Plane
      // Subtract second column of matrix from the fourth column
      planes[PLANE_BOTTOM].normal = Vector(
        matrix[0][3] + matrix[0][1],
        matrix[1][3] + matrix[1][1],
        matrix[2][3] + matrix[2][1]
      );
      planes[PLANE_BOTTOM].d = matrix[3][3] + matrix[3][1];

      // Top Frustum Plane
      // Add second column of the matrix to the fourth column
      planes[PLANE_TOP].normal = Vector(
        matrix[0][3] - matrix[0][1],
        matrix[1][3] - matrix[1][1],
        matrix[2][3] - matrix[2][1]
      );
      planes[PLANE_TOP].d = matrix[3][3] - matrix[3][1];

      // Far Frustum Plane
      // We could add the third column to the fourth column to get the near plane,
      // but we don't have to do this because the third column IS the near plane
      planes[PLANE_FAR].normal = Vector(
        matrix[0][3] - matrix[0][2],
        matrix[1][3] - matrix[1][2],
        matrix[2][3] - matrix[2][2]
      );
      planes[PLANE_FAR].d = matrix[3][3] - matrix[3][2];

      // Near Frustum Plane
      // Subtract third column of matrix from the fourth column
      planes[PLANE_NEAR].normal = Vector(
        matrix[0][3] + matrix[0][2],
        matrix[1][3] + matrix[1][2],
        matrix[2][3] + matrix[2][2]
      );
      planes[PLANE_NEAR].d = matrix[3][3] + matrix[3][2];

      // Normalize them
      for (int i=0; i < 6; i++){
          float invl = sqrt( 
            planes[i].normal.x * planes[i].normal.x +
            planes[i].normal.y * planes[i].normal.y +
            planes[i].normal.z * planes[i].normal.z);
          planes[i].normal.x /= invl;
          planes[i].normal.y /= invl;
          planes[i].normal.z /= invl;
          planes[i].d /= invl;
      }
    }

    void debug() {
      std::cout << "Frustum: " << std::endl;
      std::cout << "\t";
      std::cout << "Right: ";
      planes[PLANE_RIGHT].debug();
      std::cout << "\t";
      std::cout << "Left: ";
      planes[PLANE_LEFT].debug();
      std::cout << "\t";
      std::cout << "Top: ";
      planes[PLANE_TOP].debug();
      std::cout << "\t";
      std::cout << "Bottom: ";
      planes[PLANE_BOTTOM].debug();
      std::cout << "\t";
      std::cout << "Near: ";
      planes[PLANE_NEAR].debug();
      std::cout << "\t";
      std::cout << "Far: ";
      planes[PLANE_FAR].debug();
    }
 
    Plane planes[6];

  };

}

#endif //MATH_FRUSTRUM_H