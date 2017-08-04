#include "stdafx.h"
#include "Vertex.h"
#include "Consts.h"

namespace pong {
  cb::gl::CVertexDefinition CVertex::Def{
    {render::IDX_VERTEX4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), 0},
    {render::IDX_COLOR4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec4)},
  };
}