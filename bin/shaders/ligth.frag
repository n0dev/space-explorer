#version 400
 
layout(vertices=3) out;
 
void main(void) {
        gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
 
        gl_TessLevelOuter[0] = 1.0;
        gl_TessLevelOuter[1] = 1.0;
        gl_TessLevelOuter[2] = 1.0;
        gl_TessLevelInner[0] = 1.0;
        gl_TessLevelInner[1] = 1.0;
}