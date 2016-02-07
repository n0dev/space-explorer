varying vec4 diffuse, ambientGlobal, ambient, ecPos;
varying vec3 normal, halfVector;

uniform sampler2D tex;
uniform vec3 sunPosition;

void main()
{
    vec3 n, halfV, viewV, lightDir;
    float NdotL, NdotHV;
    vec4 color = ambientGlobal;
    float att, dist;

    /* a fragment shader can't write a varying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);

    // Compute the light direction
    //lightDir = vec3(gl_LightSource[0].position-ecPos);
    lightDir = sunPosition;

    /* compute the distance to the light source to a varying variable*/
    dist = length(lightDir);

    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n, normalize(lightDir)), 0.0);

    if (NdotL > 0.0) {

        att = 1.0 / (gl_LightSource[0].constantAttenuation +
                gl_LightSource[0].linearAttenuation * dist +
                gl_LightSource[0].quadraticAttenuation * dist * dist);
        color += att * (diffuse * NdotL + ambient);


        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV), 0.1);
        color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
    }

    vec4 texel;
    texel = texture2D(tex, gl_TexCoord[0].st);

    gl_FragColor = texel.rgba * color;
}

/*varying vec3 lightDir,normal;
uniform sampler2D tex;

void main()
{
    vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;
    intensity = max(dot(lightDir,normalize(normal)),0.0);

    cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
    af = gl_FrontMaterial.diffuse.a;
    texel = texture2D(tex,gl_TexCoord[0].st);

    ct = texel.rgb;
    at = texel.a;
    gl_FragColor = vec4(ct * cf, at * af);

}*/
