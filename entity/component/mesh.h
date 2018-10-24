#ifndef ENTITY_COMPONENT_MESH_H
#define ENTITY_COMPONENT_MESH_H

#include <vector>

#include "entity/component.h"
#include "entity/vector.h"

typedef std::vector<Vector> VertexVector;
typedef std::vector<unsigned int> IndexVector;

class Mesh : public Component {

public:

  Mesh(std::string id, VertexVector vertices, VertexVector normals, VertexVector uvs, IndexVector indices) 
    : Component("Mesh"), id(id), vertices(vertices), normals(normals), uvs(uvs), indices(indices) {}
  
  virtual ~Mesh();

  inline unsigned int getElementCount() {
    return indices.size();
  }

  inline VertexVector* getVertices() {
    return &vertices;
  }

  inline VertexVector* getNormals() {
    return &normals;
  }

  inline VertexVector* getUvs() {
    return &uvs;
  }

  inline IndexVector* getIndices() {
    return &indices;
  }

  inline std::string getId() const {
    return id;
  }

  static Mesh Cube;

  static Mesh Quad;
 
protected:

  std::string id;

  VertexVector vertices;
  
  VertexVector normals;

  VertexVector uvs;

  IndexVector indices;

};


#endif //ENTITY_COMPONENT_MESH_H