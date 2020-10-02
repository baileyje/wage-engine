#version 450
#extension GL_ARB_separate_shader_objects:enable

layout(set=0,binding=0) uniform UniformBufferObject{
  mat4 proj;
} ubo;

layout(push_constant) uniform PushConsts {
  vec2 offset;
  vec4 color;
} pushConsts;

layout(location = 0) in vec2 inPosition;
layout(location = 1)  in vec2 inTextCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main(){
  gl_Position = ubo.proj * vec4(pushConsts.offset + inPosition, 0, 1.);
  fragColor = pushConsts.colora;
  fragTexCoord = inTextCoord;
}