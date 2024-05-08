uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition;

uniform highp float u_lightPower;

varying highp vec4 v_position;
varying highp vec3 v_normal;
varying highp vec3 v_color;
varying highp vec3 v_eyePosition;
void main(void)
{
    vec4 resultColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 eyePos = vec4(v_eyePosition, 1.0f);
    vec4 diffMatColor = vec4(v_color, 0.0f);
    vec3 eyeVect = normalize(v_position.xyz - eyePos.xyz);
    vec3 lightVec = normalize(v_position.xyz - u_lightPosition.xyz);
    vec3 reflectLight = normalize(reflect(lightVec, v_normal));
    float len = length(v_position.xyz - eyePos.xyz);
    float specularFactor = 10.0f;
    float ambientFactor = 0.1f;

    vec4 diffColor = diffMatColor * u_lightPower * max(0.0f, dot(v_normal, eyePos.xyz - lightVec));// / (1.0 + 0.25 * len * len);
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    vec4 specularColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * u_lightPower * pow(max(0.0f, dot(reflectLight, -eyeVect)), specularFactor);
    resultColor += specularColor;
    gl_FragColor = resultColor;
}
