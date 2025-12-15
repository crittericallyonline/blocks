// this is not an A.I Generated game :3

#include <stdio.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5_webgl.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <math.h>

// :3 - Willow

#include "cglm/cglm.h"
#include "program.h"
#include "shader.h"
#include "objects.h"

#define PI 3.141592653589

#ifndef clamp
#define clamp(v, min, max) ((v) > (max) ? (max) : ((v) < (min) ? (min) : (v)))
#endif
#ifndef loop
#define loop(v, min, max) ((v) > (max) ? (min) : ((v) < (min) ? (max) : (v)))
#endif

// special thanks to -> Malicious chromosoze

Object *cube;
struct Camera {
    // transformations
    vec3 last_position;
    vec3 position;
    //
    vec3 velocity;
    vec3 friction;
    // vec3 acceleration;

    vec3 rotation;
    vec3 scale;

    // mat4
    mat4 transformation;
    mat4 projection;

    float playerSpeed;

    float fieldOfView;
    float nearPlane, farPlane; // define for later use in rendering
} Camera;

bool MOUSE_CALLBACK(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    switch (eventType)
    {
    case EMSCRIPTEN_EVENT_MOUSEMOVE:
        Camera.rotation[0] += mouseEvent->movementY / 250.0f;
        Camera.rotation[1] += mouseEvent->movementX / 250.0f;

        Camera.rotation[0] = clamp(Camera.rotation[0], -PI/2.f, PI/2.f);
        Camera.rotation[0] = loop(Camera.rotation[0], -PI/2.f, PI/2.f);
        
        return true;
        break;

    case EMSCRIPTEN_EVENT_DBLCLICK:
        emscripten_request_pointerlock("canvas", false);
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

bool RESIZE_CALLBACK(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_set_canvas_element_size("#canvas", uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
    glViewport(0, 0, uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
    glm_perspective((Camera.fieldOfView / 180.0f) * PI, (float) uiEvent->windowInnerWidth / (float) uiEvent->windowInnerHeight, 0.1f, 1000.0f, Camera.projection);
    return true;
}

bool pressedKeys[256] = {0};

EmscriptenFullscreenStrategy fullscreen;

bool KEYBOARD_CALLBACK(int eventType, const EmscriptenKeyboardEvent *event, void *userData)
{

    if(event->keyCode > 256)
    {
        return false;
    }

    switch (eventType)
    {

    case EMSCRIPTEN_EVENT_KEYDOWN:
        if(event->keyCode == 122){
            emscripten_request_fullscreen_strategy("#canvas", true, &fullscreen);

            fullscreen.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_CENTER;
            
        }
        pressedKeys[event->keyCode] = true;
        break;
    
    case EMSCRIPTEN_EVENT_KEYUP:
        pressedKeys[event->keyCode] = false;
        break;
    
    default:
        break;
    }
    return true;
}

void draw()
{
    // mat4 inverse_matrix; // removing unused code
    // double now = emscripten_performance_now() / 1000; // we dont need to have javascript calls
    // glClearColor(sinf(now) / 2.f + 0.5f, 0.0, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // i think this is all we do here, but before polling we must render stuff

    glUniform1i(Program.time, (int) emscripten_performance_now());

    drawModel(cube);
    

    glm_mat4_identity(Camera.transformation);
    

    // order of YXZ
    glm_rotate_x(Camera.transformation, Camera.rotation[0], Camera.transformation);
    glm_rotate_y(Camera.transformation, Camera.rotation[1], Camera.transformation);
    glm_rotate_z(Camera.transformation, Camera.rotation[2], Camera.transformation);


    glm_translate(Camera.transformation, Camera.position);


    glm_scale(Camera.transformation, Camera.scale);


    glUniformMatrix4fv(Program.modelview_matrix, 1, GL_FALSE, Camera.transformation[0]);
    glUniformMatrix4fv(Program.projection_matrix, 1, GL_FALSE, Camera.projection[0]);

    glfwPollEvents();
    glFlush();
}


void update()
{


    if(pressedKeys['W'])
    {
        Camera.velocity[2] += cos(Camera.rotation[1]) * Camera.playerSpeed;
        Camera.velocity[0] -= sin(Camera.rotation[1]) * Camera.playerSpeed;
    }
    if(pressedKeys['S'])
    {
        Camera.velocity[2] -= cos(Camera.rotation[1]) * Camera.playerSpeed;
        Camera.velocity[0] += sin(Camera.rotation[1]) * Camera.playerSpeed;
    }
    if(pressedKeys['A'])
    {
        Camera.velocity[2] += sin(Camera.rotation[1]) * Camera.playerSpeed;
        Camera.velocity[0] += cos(Camera.rotation[1]) * Camera.playerSpeed;
    }
    if(pressedKeys['D'])
    {
        Camera.velocity[2] -= sin(Camera.rotation[1]) * Camera.playerSpeed;
        Camera.velocity[0] -= cos(Camera.rotation[1]) * Camera.playerSpeed;
    }
    if(pressedKeys[' '])
    {
        Camera.velocity[1] -= Camera.playerSpeed;
    }
    if(pressedKeys[16])
    {
        Camera.velocity[1] += Camera.playerSpeed;
    }

    glm_vec3_mul(Camera.velocity, Camera.friction, Camera.velocity);
    glm_vec3_add(Camera.position, Camera.velocity, Camera.position);
}

void renderLoop()
{
    draw();
    update();

}

EMSCRIPTEN_KEEPALIVE
void set_position(int x, int y, int z)
{
    Camera.position[0] = x;
    Camera.position[1] = y;
    Camera.position[2] = z;
}

int main()
{

    double width, height;
    emscripten_get_element_css_size("#canvas", &width, &height);
    
    Camera.nearPlane = 0.1;
    Camera.farPlane = 1000;
    Camera.fieldOfView = 70;
    Camera.position[2] = -5; // backwards
    Camera.playerSpeed = 0.0025;


    glm_vec3_fill(Camera.friction, 0.89f);
    glm_vec3_fill(Camera.scale, 1);


    //void glm_perspective(float fovy, float aspect, float nearVal, float farVal, mat4 dest)

    glm_perspective((Camera.fieldOfView / 180.0f) * PI, width / height, Camera.nearPlane, Camera.farPlane, Camera.projection);
    
    glm_mat4_identity(Camera.transformation);


    if(!glfwInit())
    {
        printf("Error with creating the webgl context.\n");
        return -1;
    }

    EmscriptenWebGLContextAttributes attrs;

    emscripten_webgl_init_context_attributes(&attrs);
    
    attrs.majorVersion = 2;
    attrs.minorVersion = 2;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);

    int success = emscripten_webgl_make_context_current(context);

    if(success)
    {
        printf("error setting the webgl context to current\n");
        return -1;
    }
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);

    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, MOUSE_CALLBACK);
    emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, MOUSE_CALLBACK);
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, RESIZE_CALLBACK);
    Program.shaderProgram = gen_program("/shader/vertex.vs", "/shader/fragment.fs");
    Program.modelview_matrix = glGetUniformLocation(Program.shaderProgram, "modelview_matrix");
    Program.projection_matrix = glGetUniformLocation(Program.shaderProgram, "projection_matrix");
    Program.vertex_position = glGetAttribLocation(Program.shaderProgram, "vertex_position");
    Program.texcoord = glGetAttribLocation(Program.shaderProgram, "texcoord");
    Program.time = glGetUniformLocation(Program.shaderProgram, "iTime");

    init_textures();

    cube = create_cube(0);

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, KEYBOARD_CALLBACK);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, KEYBOARD_CALLBACK);

    
    emscripten_set_canvas_element_size("#canvas", (int) width, (int) height);
    glViewport(0, 0, (int) width, (int) height);


    emscripten_set_main_loop(renderLoop, 0, true);

    return 0;
}