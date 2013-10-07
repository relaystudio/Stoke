uniform float t;
uniform float multi;

void main (void) {
	
	vec4 one = vec4(sin((gl_FragCoord.y * 0.1 - (cos(t) * 3.) - 10.) * 0.1 - t) + 0.75,
					sin((gl_FragCoord.y * 0.1 - 15.) * 0.1 - t - 0.2) - sin(t) * 5. + 0.15,
					sin((gl_FragCoord.y * 0.1 - 20.) * 0.1 - t - 0.3) - cos(t * 2.) + 0.65,
					sin(gl_FragCoord.y * 0.5 - t)  + cos(gl_FragCoord.x * 0.01 + t));
	
	vec4 two = vec4(cos(-t) + 0.1,
					sin(cos(t)) * 0.5,
					abs(cos(t)),
					sin(-t + (gl_FragCoord.y * 0.02)));
	
	vec4 three = vec4(sin(cos(-t)), cos(-t) * 0.1, tan(t), sin(-t));
	
	gl_FragColor = (one + two * three) * multi;
}