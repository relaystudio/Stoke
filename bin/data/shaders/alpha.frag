#version 120

varying vec2 texCoord;
uniform sampler2DRect tex0, tex1;


void main(void) {
    
    float mask = texture2DRect(tex1, gl_FragCoord.xy).r;
    vec4 clr = texture2DRect(tex0, gl_FragCoord.xy).rgba;
    gl_FragColor = vec4(clr.r, clr.g, clr.b, mask);

//    if (mask > .9)
//        gl_FragColor = texture2DRect(tex0, gl_FragCoord.xy).rgba;
//    else
//        gl_FragColor = texture2DRect(tex1, gl_FragCoord.xy).rgba;
}