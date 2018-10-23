#version 330 core

out vec4 fragColor;

in vec3 fragVert;
in vec3 fragNormal;

uniform mat4 model;
uniform vec3 viewPos;
uniform vec4 objectColor;

#define MAX_LIGHTS 10
int numLights = 1;
uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec3 coneDirection;
} allLights[MAX_LIGHTS];

float materialShininess = 0.0;
vec3 materialSpecularColor = vec3(0);

vec3 applyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
  vec3 surfaceToLight;
  float attenuation = 1.0;
  // if(light.position.w == 0.0) {
  //     //directional light
      surfaceToLight = -normalize(light.position.xyz);
      attenuation = 1.0; //no attenuation for directional lights
  // } else {
  //     //point light
  //     surfaceToLight = normalize(light.position.xyz - surfacePos);
  //     float distanceToLight = length(light.position.xyz - surfacePos);
  //     attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

  //     //cone restrictions (affects attenuation)
  //     float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
  //     if(lightToSurfaceAngle > light.coneAngle){
  //         attenuation = 0.0;
  //     }
  // }

  //ambient
  vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

  //diffuse
  float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
  vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
  
  //specular
  float specularCoefficient = 0.0;
  if(diffuseCoefficient > 0.0)
      specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
  vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

  //linear color (color before gamma correction)
  return ambient + attenuation*(diffuse + specular);
}


void main() {
  
  vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
  vec3 surfacePos = vec3(model * vec4(fragVert, 1));
  vec3 surfaceToCamera = normalize(viewPos - surfacePos);  

  //combine color from all the lights
  vec3 linearColor = vec3(0);
  for(int i = 0; i < numLights; ++i){
      linearColor += applyLight(allLights[i], objectColor.rgb, normal, surfacePos, surfaceToCamera);
  }
  
  //final color (after gamma correction)
  vec3 gamma = vec3(1.0/2.2);
  fragColor = vec4(pow(linearColor, gamma), objectColor.a);
}