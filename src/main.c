#include <stdio.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5_webgl.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>

// :3 - Willow

#include "shader.h"
#include "cglm/cglm.h"

double width, height;

struct World {
} World;

struct Program {
    GLuint shaderProgram;
    
    // uniforms
    GLuint projection_matrix;
    GLuint modelview_matrix;
} Program;

struct Camera {
    vec3 position;
    vec3 rotation;
    vec3 scale;

    mat4 transformation;
    mat4 projection;
    

    float fieldOfView;
    float nearPlane, farPlane; // define for later use in rendering
} Camera;


bool onMouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    // printf("(%d, %d)\n", mouseEvent->clientX, mouseEvent->clientY);
    return true;
}

bool RESIZE_CALLBACK(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_get_element_css_size("#canvas", &width, &height);
    glViewport(0, 0, width, height);
    return true;
}

bool KEYBOARD_CALLBACK(int eventType, const EmscriptenKeyboardEvent *event, void *userData)
{
    printf("%s\n", event->charValue);
    return true;
}

void draw()
{
    static mat4 inverse_matrix;
    double now = emscripten_performance_now() / 1000;
    // glClearColor(sinf(now) / 2.f + 0.5f, 0.0, 0.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // i think this is all we do here, but before polling we must render stuff

    glm_translate(Camera.transformation, Camera.position);
    glm_mat4_inv(Camera.transformation, inverse_matrix);
    glUniformMatrix4fv(Program.modelview_matrix, 1, GL_FALSE, inverse_matrix[0]);

    // polling and flushing, why is flush here again? i dont remember
    glfwPollEvents();
    glFlush();
}

void renderLoop()
{
    draw();
}

int main()
{
    Camera.nearPlane = 0.1;
    Camera.farPlane = 1000;
    Camera.fieldOfView = 70;
    Camera.position[2] = 5; // backwards

    //void glm_perspective(float fovy, float aspect, float nearVal, float farVal, mat4 dest)

    // thanks to
    //Malicious chromosoze
    //  — 4:41 PM
    // fovy is in RADIANS
    // aspect = width / height
    // for near and far good values are 0.1 and 1000 respectively
    glm_perspective(1.22173048f, width / height, Camera.nearPlane, Camera.farPlane, Camera.projection);

    glm_mat4_identity(Camera.transformation);


    if(!glfwInit())
    {
        printf("Error with creating the webgl context.\n");
        return -1;
    }

    EmscriptenWebGLContextAttributes attrs;
    
    attrs.majorVersion = 2;
    attrs.minorVersion = 3;

    emscripten_webgl_init_context_attributes(&attrs);
    
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);

    int success = emscripten_webgl_make_context_current(context);

    if(success)
    {
        printf("error setting the webgl context to current\n");
        return -1;
    }
    
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, onMouseMove);
    emscripten_get_element_css_size("#canvas", &width, &height);
    emscripten_set_resize_callback("#canvas", NULL, true, RESIZE_CALLBACK);
    Program.shaderProgram = gen_program("/shader/vertex.vs", "/shader/fragment.fs");
    Program.modelview_matrix = glGetUniformLocation(Program.shaderProgram, "modelview_matrix");
    Program.projection_matrix = glGetUniformLocation(Program.shaderProgram, "projection_matrix");
    
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, KEYBOARD_CALLBACK);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, KEYBOARD_CALLBACK);

    emscripten_set_main_loop(renderLoop, 0, true);

    return 0;
}