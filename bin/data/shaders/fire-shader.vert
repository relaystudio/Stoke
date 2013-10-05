attribute float pointSize;
uniform float bumpAmount;
uniform float bumpWidth;
uniform float fboWidth;

void main() {
	vec4 rawPos = gl_Vertex;
	float bump = sin((rawPos.x / fboWidth) * 3.14159);
	rawPos.y -= bump * bumpAmount;
	rawPos.y += bumpWidth;
    gl_Position   = gl_ModelViewProjectionMatrix * rawPos;
	gl_PointSize  = pointSize;
    gl_FrontColor = gl_Color;
}