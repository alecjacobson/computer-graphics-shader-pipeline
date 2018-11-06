#ifndef CREATE_SHADER_PROGRAM_H
#define CREATE_SHADER_PROGRAM_H
#include <string>
#include "gl.h"

inline bool create_shader_program(
  const std::string & vert_source,
  const std::string & tess_control_source,
  const std::string & tess_eval_source,
  const std::string & frag_source,
  GLuint & id);

// Implementation

#include <iostream>
#include "REDRUM.h"
#include "STR.h"
#include "find_and_replace_all.h"
#include "print_shader_info_log.h"
#include "print_program_info_log.h"

inline bool create_shader_program(
  const std::string & vert_source,
  const std::string & tess_control_source,
  const std::string & tess_eval_source,
  const std::string & frag_source,
  GLuint & id)
{

  const auto create_compile_attach = [](
    const std::string & src, 
    const GLenum type, 
    const GLuint prog_id, 
    GLuint & s) -> bool
  {
    const std::string type_str = 
      (type == GL_VERTEX_SHADER ?                STR(BOLD("vertex shader")) :
        (type == GL_FRAGMENT_SHADER ?            STR(BOLD("fragment shader")) :
          (type == GL_TESS_CONTROL_SHADER ?      STR(BOLD("tessellation control shader")) :
            (type == GL_TESS_EVALUATION_SHADER ? STR(BOLD("tessellation evaluation shader")) : 
               "unknown shader"))));
    if(src == "")
    {
      std::cerr<<YELLOWRUM("WARNING")<<": "<<type_str<<" is empty..."<<std::endl;;
      s = 0;
      return false;
    }
    s = glCreateShader(type);
    if(s == 0)
    {
      std::cerr<<"failed to create "<<type_str<<std::endl;
      return false;
    }
    {
      const char *c = src.c_str();
      glShaderSource(s,1,&c,NULL);
    }
    glCompileShader(s);
    print_shader_info_log(type_str,s);
    if(!glIsShader(s))
    {
      std::cerr<<type_str<<" failed to compile."<<std::endl;
      return false;
    }
    glAttachShader(prog_id,s);
    return true;
  };

  // create program
  id = glCreateProgram();
  if(id == 0)
  {
    std::cerr<<REDRUM("ERROR")<<": could not create shader program."<<std::endl;
    return false;
  }
  GLuint v=0,tc=0,te=0,f=0;

  create_compile_attach(vert_source,GL_VERTEX_SHADER,id,v);
  create_compile_attach(tess_control_source,GL_TESS_CONTROL_SHADER,id,tc);
  create_compile_attach(tess_eval_source,GL_TESS_EVALUATION_SHADER,id,te);
  create_compile_attach(frag_source,GL_FRAGMENT_SHADER,id,f);

  // Link program
  glLinkProgram(id);
  const auto & detach = [&id](const GLuint shader)
  {
    if(shader)
    {
      glDetachShader(id,shader);
      glDeleteShader(shader);
    }
  };
  detach(f);
  detach(v);

  // print log if any
  print_program_info_log(id);
  GLint status;
  glGetProgramiv(id,GL_LINK_STATUS,&status);
  if(status != GL_TRUE)
  {
    std::cerr<<REDRUM("ERROR")<<": Failed to link shader program"<<std::endl;
    return false;
  }

  std::cout<<GREENRUM("shader compilation successful.")<<std::endl;
  return true;
}

#endif
