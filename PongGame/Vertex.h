#pragma once

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <CBGL/VertexDefinition.h>

namespace pong {
  struct CVertex {
    glm::vec2 mPos;
    glm::vec2 mTex;
    glm::vec4 mColor = {1.0f, 1.0f, 1.0f, 1.0f};

    CVertex() {}
    CVertex(float x, float y, float s = 0.0f, float t = 0.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
      : CVertex(x, y, s, t, glm::vec4(r, g, b, a)) {}
    CVertex(float x, float y, float s, float t, glm::vec4 const& color)
      : mPos(x, y), mTex(s, t), mColor(color) {}

    static cb::gl::CVertexDefinition Def;
  };
}