#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    //FragColor = texture(texture1, TexCoords);
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.r <0.2)
        discard;
    FragColor=texColor;


}