#include "Vertex.h"

namespace pong {
  cb::gl::CVertexDefinition CVertex::Def{
    {0, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), 0},
    {1, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec4)},
  };
}