#version 450
#extension GL_ARB_separate_shader_objects:enable

layout(set=0, binding=0)uniform UniformBufferObject {
  mat4 view;
  mat4 proj;
} ubo;

layout(push_constant)uniform PushConsts {
  mat4 model;
} primitive;


layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 ignoredColor;
layout(location=2)in vec3 ignoredNormal;

void main(){
  gl_Position=ubo.proj*ubo.view*primitive.model*vec4(inPosition,1.);  
}