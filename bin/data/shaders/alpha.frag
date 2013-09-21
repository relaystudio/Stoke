#version 120
uniform sampler2D tex;
uniform sampler2D mask;

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
    vec4 tex = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
	vec4 mask = texture2D(mask, gl_TexCoord[0].st) * gl_Color;
	//we use the mod function to only draw pixels if they are every 2 in x or every 4 in y
//    if(tex.a >= 1.0) {
//		gl_FragColor = vec4(0.,0.,0.,0.);
//    }else{
//		gl_FragColor = vec4(1.,1.,1.,1.);
//	}
    if(mask.r >= .9) {
        gl_FragColor = tex;
    } else {
        gl_FragColor = vec4(0.,0.,0.,0.);
    }
    
}