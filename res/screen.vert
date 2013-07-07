#version 330 core
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 color;

out vec3 position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Normal_cameraspace;

void main(){
  mat4 MVP = P * V * M;
  gl_Position = MVP * vec4(vertexPosition, 1);

  position_worldspace = (M * vec4(vertexPosition, 1)).xyz;

  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition, 1)).xyz;
  EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
   
  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 LightPosition_cameraspace = ( V * vec4(-2, 2, -2, 1)).xyz;
  LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
   
  // Normal of the the vertex, in camera space
  Normal_cameraspace = ( V * inverse(transpose(M)) * vec4(vertexNormal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
