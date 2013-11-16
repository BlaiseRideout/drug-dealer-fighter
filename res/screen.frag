#version 330 core
uniform sampler2D tex;

in vec3 position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;
in vec2 fragUV;

out vec4 vertColor;

void main(){
  vec3 n = normalize(Normal_cameraspace);
  vec3 l = normalize(LightDirection_cameraspace);

  // Eye vector (towards the camera)
  vec3 E = normalize(EyeDirection_cameraspace);
  // Direction in which the triangle reflects the light
  vec3 R = reflect(-l,n);
  // Cosine of the angle between the Eye vector and the Reflect vector,
  // clamped to 0
  //  - Looking into the reflection -> 1
  //  - Looking elsewhere -> < 1
  float cosAlpha = clamp( dot( E,R ), 0,1 );

  vec3 MaterialDiffuseColor = texture2D(tex, vec2(fragUV.x, fragUV.y)).rgb;
  vec3 MaterialAmbientColor = vec3(0.4, 0.4, 0.4) * MaterialDiffuseColor;

  float distance = length(LightDirection_cameraspace);

  float cosTheta = clamp(dot( n,l ), 0, 1);
  vec3 color = MaterialDiffuseColor * vec3(1.0f, 1.0f, 1.0f) * 10 * cosTheta / pow(distance, 2);

  color = vec3(1.0f, 1.0f, 1.0f) * .5 * pow(cosAlpha, 5) / pow(distance, 2) + color;

  vertColor = vec4(MaterialAmbientColor + color, 1.0f);
}
