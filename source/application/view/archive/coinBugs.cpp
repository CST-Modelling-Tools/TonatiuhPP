
void
SoShadowGroupP::setVertexShader(SoState * state)
{
  int i;
  SoShaderGenerator & gen = this->vertexgenerator;
  gen.reset(FALSE);
  gen.setVersion("#version 120");
  
 
   SoShaderGenerator & vgen = this->vsm_vertex_generator;
  SoShaderGenerator & fgen = this->vsm_fragment_generator;

  vgen.reset(FALSE);
  vgen.setVersion("#version 120");

  SbBool dirlight = this->light->isOfType(SoDirectionalLight::getClassTypeId());

  vgen.addDeclaration("varying vec3 light_vec;", FALSE);
  vgen.addMainStatement("light_vec = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
                        "gl_Position = ftransform();");

  vshader->sourceProgram = vgen.getShaderProgram();
  vshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  fgen.reset(FALSE);
  fgen.setVersion("#version 120");
  
  
  better inside VertexGener