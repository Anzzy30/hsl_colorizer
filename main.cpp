#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glad/gl.h"
#include "hsl_colorizer.hpp"
#include <iostream>
struct vertex
{
    float x, y;
    float r, g, b;
} ;

//coord -1 1
std::vector<vertex> create_quad(glm::vec2 center, float res, hsl_colorizer::rgb rgb){
    float half_res = res/2.f;
    std::vector<vertex> v(4);

    v[0] = {center.x-half_res, center.y-0.25f, rgb.r,rgb.g,rgb.b};
    v[1] = {center.x-half_res, center.y+0.25f, rgb.r,rgb.g,rgb.b};
    v[2] = {center.x+half_res, center.y+0.25f, rgb.r,rgb.g,rgb.b};
    v[3] = {center.x+half_res, center.y-0.25f, rgb.r,rgb.g,rgb.b};

    return v;

}

std::vector<vertex> generate_data(){

    float min = -1, max=1;
    int n = 1000;
    std::vector<vertex> vertices(n*4);
    float res = 2.f/(float)n;
    hsl_colorizer::rgb color;
    for(int i = 0; i < n ; ++i)
    {

        float x = hsl_colorizer::normalize(i,0,n);
        color = hsl_colorizer::full_lerp_hsl(x,1.f,0.5f);
//        std::vector<vertex> quad = create_quad(glm::vec2(x,0),res,);
//        color = hsl_colorizer::jethsl(x,0.95f,0.75f);
//        color = hsl_colorizer::jet(x);
//        color = hsl_colorizer::hsl_morgantrouvesaboxd(x);
        x =x*2.f-1.f;

        std::vector<vertex> quad = create_quad(glm::vec2(x,0),res,color);



        vertices.insert(vertices.begin()+i*4,quad.begin(),quad.end());
    }
    return vertices;
}

static const char* vertex_shader_text =
        "#version 110\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragment_shader_text =
        "#version 110\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Color Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
    std::vector<vertex> vertices = generate_data();
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*vertices.size(),vertices.data() , GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 m(1), p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);


        //        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = p*m;
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp[0][0]);
        glDrawArrays(GL_QUADS, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
