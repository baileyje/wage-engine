#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;

out vec3 fragVert;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  fragVert = vec3(model * vec4(vertPos, 1.0));
  fragNormal = mat3(transpose(inverse(model))) * vertNormal;
  gl_Position = projection * view * vec4(fragVert, 1.0);
}