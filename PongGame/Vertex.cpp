#include "stdafx.h"
#include "Vertex.h"
#include "Consts.h"

namespace pong {
  cb::gl::CVertexDefinition CVertex::Def{
    {IDX_VERTEX4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), 0},
    {IDX_COLOR4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec4)},
  };
}