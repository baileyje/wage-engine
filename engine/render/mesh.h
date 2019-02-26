#ifndef ENTITY_COMPONENT_MESH_H
#define ENTITY_COMPONENT_MESH_H

#include <vector>
#include <algorithm>
#include <cmath>

#include "math/vector.h"

namespace wage {
  typedef std::vector<Vector> VertexVector;
  typedef std::vector<Vector2> Vertex2Vector;
  typedef std::vector<unsigned int> IndexVector;

  class Mesh {

  public:

    Mesh()  {
    }

    Mesh(std::string id, VertexVector vertices, VertexVector normals, Vertex2Vector uvs, IndexVector indices) 
      : _id(id), _vertices(vertices), _normals(normals), _uvs(uvs), _indices(indices), _maxDim(0) {
        for (auto vertex : _vertices) {
          _maxDim.x = std::max(_maxDim.x, std::abs(vertex.x));
          _maxDim.y = std::max(_maxDim.y, std::abs(vertex.y));
          _maxDim.z = std::max(_maxDim.z, std::abs(vertex.z));
        }
      }

    Mesh(Mesh* templateMesh) 
      : _id(templateMesh->_id), _vertices(templateMesh->_vertices), _normals(templateMesh->_normals), _uvs(templateMesh->_uvs), _indices(templateMesh->_indices), _maxDim(0) {
        for (auto vertex : _vertices) {
          _maxDim.x = std::max(_maxDim.x, std::abs(vertex.x));
          _maxDim.y = std::max(_maxDim.y, std::abs(vertex.y));
          _maxDim.z = std::max(_maxDim.z, std::abs(vertex.z));
        }
      }
    
    virtual ~Mesh();

    inline unsigned int elementCount() {
      return _indices.size();
    }

    inline VertexVector& vertices() {
      return _vertices;
    }

    inline VertexVector& normals() {
      return _normals;
    }

    inline Vertex2Vector& uvs() {
      return _uvs;
    }

    inline IndexVector& indices() {
      return _indices;
    }

    inline std::string id() const {
      return _id;
    }

    inline Vector maxDim() {
      return _maxDim; 
    }

    static void generatePrimitives();

    static Mesh* load(std::string path);
  
    static Mesh* Cube;
    
    static Mesh* Sphere;

    static Mesh* Quad;

  protected:

    std::string _id;

    VertexVector _vertices;
    
    VertexVector _normals;

    Vertex2Vector _uvs;

    IndexVector _indices;

    Vector _maxDim;

  };

}

#endif //ENTITY_COMPONENT_MESH_H