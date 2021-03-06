#include "stdafx.h"
#include "GFXShaderRepository.h"
#include "GFXConsts.h"

#include <CBStr/StringEx.h>
#include <CBIO/File.h>
#include <CBGL/Shader.h>
#include <CBGL/Program.h>

namespace gfx {
  CShaderRepository::CShaderRepository(cb::string const & assetsDir) 
    : core::CAssetRepository<cb::gl::CProgram>(assetsDir, L"glsl"s)
  {}

  CShaderRepository::~CShaderRepository() {}

  std::shared_ptr<cb::gl::CProgram> CShaderRepository::Load(cb::string const & name) const {
    auto shaders = std::vector<cb::gl::CShader>();
    auto shadernames = cb::split(name, L","s);
    for(auto& shadername : shadernames) {
      auto type = FindShaderType(shadername);
      auto path = GetAssetPath(shadername);

      shaders.push_back(LoadShader(type, path));
    }

    return CreateProgram(shaders);
  }

  std::shared_ptr<cb::gl::CProgram> CShaderRepository::CreateProgram(std::vector<cb::gl::CShader>& shaders) const {
    auto program = cb::gl::CProgram();
    program.Attach(shaders);
    program.SetInLocation({
      {gfx::IDX_VERTEX4, gfx::VIN_VERTEX4},
      {gfx::IDX_COLOR4, gfx::VIN_COLOR4}
    });
    program.Link();
    if(!program.IsLinked()) {
      cb::warn(L"Failed to link gl program.");
      cb::info(program.GetLinkLog());
      throw std::exception("GL program compilation failed.");
    }

    return std::make_shared<cb::gl::CProgram>(std::move(program));
  }

  cb::gl::CShader CShaderRepository::LoadShader(cb::gl::ShaderType const type, cb::string const & path) const {
    cb::info(L"Compiling shader from file {0}", path);
    auto source = cb::readtextfileutf8(path);

    auto shader = cb::gl::CShader(type, source);
    if(!shader.IsCompiled()) {
      cb::warn(L"Shader compilation failed.");
      cb::info(shader.GetCompileLog());
      throw std::exception("Shader compilation failed.");
    }

    return shader;
  }

  cb::gl::ShaderType CShaderRepository::FindShaderType(cb::string const & name) const {
    if(cb::ends_with(name, L"_vs"s)) { return cb::gl::ShaderType::VERTEX; }
    if(cb::ends_with(name, L"_fs"s)) { return cb::gl::ShaderType::FRAGMENT; }
    return cb::gl::ShaderType::VERTEX;
  }
}
