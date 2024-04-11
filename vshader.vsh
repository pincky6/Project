attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute highp vec3 a_color;

uniform highp vec3 u_eyePosition;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;

varying highp vec4 v_position;
varying highp vec3 v_normal;
varying highp vec3 v_color;
varying highp vec3 v_eyePosition;

void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;

    gl_Position = u_projectionMatrix * mv_matrix * a_position;

    v_color = a_color;
    v_eyePosition = u_eyePosition;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;

}
