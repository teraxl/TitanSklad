uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

void main(void)
{
    vec4 resultColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 eyePosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition);
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));
    float len = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = 60.0f;
    float ambientFactor = 0.1f;

    vec4 diffColor = diffMatColor * u_lightPower
            * max(0.0f, dot(v_normal, -lightVect))/* / (1.0f + 0.25 * pow(len, 2))*/;
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f)
            * u_lightPower * pow(max(0.0f, dot(reflectLight, -eyeVect)), specularFactor)
            /*/ (1.0f + 0.25 * pow(len, 2))*/;
    resultColor += specularColor;
    gl_FragColor = resultColor;
}
