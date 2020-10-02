
#version 450
#extension GL_ARB_separate_shader_objects:enable

layout(set=1, binding=0)uniform sampler2D texSampler;

layout(location=0)in vec4 textColor;
layout(location=1)in vec2 fragTexCoord;

layout(location=0)out vec4 color;

void main() {
  vec2 flippedTexcoord = vec2(fragTexCoord.x, 1.0 - fragTexCoord.y);
  vec4 sampled = vec4(1.,1.,1., texture(texSampler, flippedTexcoord).r);
  color = textColor * sampled;
}