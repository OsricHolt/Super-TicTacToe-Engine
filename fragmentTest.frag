#version 330 core
out vec4 FragColor;

uniform vec4 pieceColor;

void main()
{
   FragColor = vec4(pieceColor);
}
