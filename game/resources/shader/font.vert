#version 450
#extension GL_ARB_separate_shader_objects:enable

layout(set=0,binding=0)uniform UniformBufferObject{
  mat4 view;
  mat4 proj;
} ubo;

// layout(push_constant)uniform PushConsts{
//   vec2 offset;
// } primitive;

layout(push_constant)uniform PushConsts{
  mat4 model;
}primitive;


layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inColor;
layout(location=2)in vec3 normal;
layout(location=3)in vec2 inTextCoord;


layout(location=0)out vec3 fragColor;
layout(location=1)out vec2 fragTexCoord;

void main(){
  // gl_Position=vec4(inPosition.xy , 0. ,1.);
  gl_Position=ubo.proj * primitive.model * vec4(inPosition,1.);
  fragColor=inColor;
  fragTexCoord=inTextCoord;
}