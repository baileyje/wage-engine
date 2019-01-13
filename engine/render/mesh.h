#ifndef ENTITY_COMPONENT_MESH_H
#define ENTITY_COMPONENT_MESH_H

#include <vector>
#include <algorithm>
#include <cmath>

#include "entity/component.h"
#include "math/vector.h"

namespace wage {
  typedef std::vector<Vector> VertexVector;
  typedef std::vector<Vector2> Vertex2Vector;
  typedef std::vector<unsigned int> IndexVector;

  class Mesh : public Component {

  public:

    Mesh(std::string id, VertexVector vertices, VertexVector normals, Vertex2Vector uvs, IndexVector indices) 
      : Component("Mesh"), id_(id), vertices_(vertices), normals_(normals), uvs_(uvs), indices_(indices), maxDim_(0) {
        for (auto vertex : vertices_) {
          // printf("Vert:%s - %f:%f:%f\n", id.c_str(), vertex.x, vertex.y, vertex.z);
          maxDim_.x = std::max(maxDim_.x, std::abs(vertex.x));
          maxDim_.y = std::max(maxDim_.y, std::abs(vertex.y));
          maxDim_.z = std::max(maxDim_.z, std::abs(vertex.z));
        }
      }
    
    virtual ~Mesh();

    inline unsigned int elementCount() {
      return indices_.size();
    }

    inline VertexVector& vertices() {
      return vertices_;
    }

    inline VertexVector& normals() {
      return normals_;
    }

    inline Vertex2Vector& uvs() {
      return uvs_;
    }

    inline IndexVector& indices() {
      return indices_;
    }

    inline std::string id() const {
      return id_;
    }

    inline Vector maxDim() {
      return maxDim_; 
    }

    static Mesh Cube;

    static Mesh Quad;

    static Mesh Sphere;

    static void generatePrimitives();

    static Mesh* load(std::string path);
  
  protected:

    std::string id_;

    VertexVector vertices_;
    
    VertexVector normals_;

    Vertex2Vector uvs_;

    IndexVector indices_;

    Vector maxDim_;

  };

}

#endif //ENTITY_COMPONENT_MESH_H