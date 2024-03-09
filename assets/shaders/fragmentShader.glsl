#version 330

in vec2 texCoords;
out vec4 color;

struct DirectionalLight{
    vec3 color;
    float ambientIntensity;
};

uniform DirectionalLight directionalLight;

uniform int hasOverlayTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    vec4 ambientLight = vec4(directionalLight.color, 1.0) * directionalLight.ambientIntensity;
    
    if(hasOverlayTexture == 1){
        vec4 overlayTexture = texture(texture2, texCoords);
        vec4 mainTexture = texture(texture1, texCoords); 
        vec4 tempColor = mix(mainTexture, overlayTexture, overlayTexture.a);
        color = vec4(tempColor.x, tempColor.y, tempColor.z, mainTexture.a);
    }else{
        color = texture(texture1, texCoords);
    }

    //color = texture(texture1, texCoords);// * ambientLight;    
}