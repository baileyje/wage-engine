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
      : Component("Mesh"), id(id), vertices(vertices), normals(normals), uvs(uvs), indices(indices), maxDim(0) {
        for (auto vertex : vertices) {
          // printf("Vert:%s - %f:%f:%f\n", id.c_str(), vertex.x, vertex.y, vertex.z);
          maxDim.x = std::max(maxDim.x, std::abs(vertex.x));
          maxDim.y = std::max(maxDim.y, std::abs(vertex.y));
          maxDim.z = std::max(maxDim.z, std::abs(vertex.z));
        }
      }
    
    virtual ~Mesh();

    inline unsigned int getElementCount() {
      return indices.size();
    }

    inline VertexVector& getVertices() {
      return vertices;
    }

    inline VertexVector& getNormals() {
      return normals;
    }

    inline Vertex2Vector& getUvs() {
      return uvs;
    }

    inline IndexVector& getIndices() {
      return indices;
    }

    inline std::string getId() const {
      return id;
    }

    inline Vector getMaxDim() {
      return maxDim; 
    }

    static Mesh Cube;

    static Mesh Quad;

    static Mesh Sphere;

    static void generatePrimitives();

    static Mesh* load(std::string path);
  
  protected:

    std::string id;

    VertexVector vertices;
    
    VertexVector normals;

    Vertex2Vector uvs;

    IndexVector indices;

    Vector maxDim;

  };

}

#endif //ENTITY_COMPONENT_MESH_H