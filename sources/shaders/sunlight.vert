/*
http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/directional-lights-i/
*/

varying vec4 diffuse,ambientGlobal,ambient, ecPos;
varying vec3 normal,halfVector;

void main()
{
    vec3 aux;

    /* first transform the normal into eye space and normalize the result */
    normal = normalize(/*gl_NormalMatrix **/ gl_Normal);

    /* compute the vertex position in camera space. */
    ecPos = /*gl_ModelViewMatrix **/ gl_Vertex;

    /* Normalize the halfVector to pass it to the fragment shader */
    halfVector = gl_LightSource[0].halfVector.xyz;

    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

    /* Add texture */
    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_Position = ftransform();
}

/*varying vec3 lightDir,normal;

void main()
{
    normal = normalize(gl_NormalMatrix * gl_Normal);

    lightDir = normalize(vec3(gl_LightSource[0].position));
    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_Position = ftransform();
}*/
