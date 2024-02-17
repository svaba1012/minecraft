#version 330

in vec2 texCoords;
out vec4 color;

struct DirectionalLight{
    vec3 color;
    float ambientIntensity;
};

uniform DirectionalLight directionalLight;

uniform sampler2D theTexture;

void main(){
    vec4 ambientLight = vec4(directionalLight.color, 1.0) * directionalLight.ambientIntensity;
    
    color = texture(theTexture, texCoords);// * ambientLight;
}