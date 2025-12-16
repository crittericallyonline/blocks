#version 300 es

#define PI 3.141592653589

// in
in vec3 vertex_position;
in float vertex_id;
in vec2 texcoord;

// globals
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform int iTime;

// out
out vec2 TexCoord;

float rad(float v)
{
    return (v / 180.f) * PI;
}

void main()
{
    int index = int(vertex_id);
    vec3 pos = vertex_position;
    // int time = (iTime / 100) % 6;
    float time = (float(iTime) / 1000.0);

    pos.y += sin(rad(time) * 90.0f);

    //code goes below

    // switch(time)
    // {
    //     case 0:
    //         pos.z = 1.0;
    //         pos.x *= -1.;
    //         break;

    //     case 1:
    //         pos.z = 0.0;
    //         break;

    //     case 2:
    //         pos.z += cos(pos.y + PI / 2.0);
    //         pos.y = 0.5;
    //         break;

    //     case 3:
    //         pos.z += cos(pos.y - PI / 2.0);
    //         pos.y = -0.5;
    //         break;
            
    //     case 4:
    //         pos.z += cos(pos.x + PI / 2.0);
    //         pos.x = 0.5;
    //         break;
            
    //     case 5:
    //         pos.z += cos(pos.x - PI / 2.0);
    //         pos.x = -0.5;
    //         break;
    //     default:
    //         break;
    // }

    //code goes above



    TexCoord = vec2(texcoord.x, 1.0 - texcoord.y);
    gl_Position = projection_matrix * modelview_matrix * vec4(pos, 1.0);
}