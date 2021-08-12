#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "cloth.hpp"
#include "shader.hpp"

constexpr int WIDTH  = 1280;
constexpr int HEIGHT = 720;

void key_callback(GLFWwindow* window, int key, int, int action, int);

int main() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Cloth", nullptr, nullptr);
    if (!window) {
        std::cerr << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize GLEW" << std::endl;
        return 1;
    }

    State state{};
    glfwSetWindowUserPointer(window, &state);
    glfwSetKeyCallback(window, key_callback);

    Cloth cloth{{ -7.5f, 5.0f, 0.0f }, 15, 10, 75, 50 };
    Ball  ball{{ 0.0f, 0.0f, 1.0f }, 2.0f, { 0.0f, 1.0f, 0.0f }};

    Shader shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
    glUseProgram(shader.id);

    // the camera doesn't move, so we can compute the matrices outside the loop
    auto proj = glm::perspective(glm::radians(45.0f), (float) WIDTH / HEIGHT, 0.1f, 1000.0f);
    auto view = glm::lookAt(glm::vec3{ -10.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    shader.set("light_position", view * glm::vec4{ 0.0f, 5.0f, 20.0f, 1.0f });

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the ball
        ball.update(state);

        // add some arbitrary gravity to the cloth, collide with the ball and update the cloth
        cloth.add_force({ 0.0f, -0.09f, 0.0f });
        cloth.ball_collision(ball);
        if (state.keys[Keys::space]) {
            cloth.add_wind({ 0.0f, 0.0f, -0.01f });
        }
        cloth.update();

        // draw the ball
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, ball.position);
        auto mv = view * model;
        shader.set("mv", mv);
        shader.set("mvp", proj * mv);
        shader.set("normal_matrix", glm::transpose(glm::inverse(mv)));
        ball.draw();

        // draw the cloth
        // we don't translate to the cloth's position because its particles are already moved to world coordinates in order to interact with stuff
        model = glm::mat4{ 1.0f };
        mv    = view * model;
        shader.set("mv", mv);
        shader.set("mvp", proj * mv);
        shader.set("normal_matrix", glm::transpose(glm::inverse(mv)));
        cloth.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int, int action, int) {
    auto state = (State*) glfwGetWindowUserPointer(window);

    // if Escape is pressed, exit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    // check if a key is either pressed or released
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        // set whether it is pressed or released
        bool pressed = action == GLFW_PRESS;
        // update each key as either pressed or not pressed
        switch (key) {
            case GLFW_KEY_W:state->keys[Keys::w] = pressed;
                break;
            case GLFW_KEY_A:state->keys[Keys::a] = pressed;
                break;
            case GLFW_KEY_S:state->keys[Keys::s] = pressed;
                break;
            case GLFW_KEY_D:state->keys[Keys::d] = pressed;
                break;
            case GLFW_KEY_E:state->keys[Keys::e] = pressed;
                break;
            case GLFW_KEY_Q:state->keys[Keys::q] = pressed;
                break;
            case GLFW_KEY_SPACE:state->keys[Keys::space] = pressed;
                break;
            default: break;
        }
    }
}
