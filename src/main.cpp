#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <signal.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "renderer.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "texture.h"

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    unsigned int windowWidth = 960;
    unsigned int windowHeight = 540;

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Voxel", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSizeLimits(window, windowWidth, windowHeight, -1, -1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return 1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
              0.f,   0.f, 0.f, 0.f,
            100.f,   0.f, 1.f, 0.f,
            100.f, 100.f, 1.f, 1.f,
              0.f, 100.f, 0.f, 1.f,
        };

        unsigned int indices[] = {
            0, 1, 2, 
            2, 3, 0,
        };

        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(float) * 4 * 4);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/basic.glsl");
        shader.Bind();
        // shader.SetUniform4f("uColor", 0.4f, 0.8f, 0.3f, 1.0f);
        shader.SetUniformMat4f("uModel", mvp);

        Texture texture("res/textures/logo.png");
        texture.Bind();
        shader.SetUniform1i("uTexture", 0);

        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // Init ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
        ImGui::StyleColorsDark();

        glm::vec3 translation(200, 200, 0);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("uModel", mvp);

            {
                ImGui::SliderFloat("X", &translation.x, 0.f, 800.f);
                ImGui::SliderFloat("Y", &translation.y, 0.f, 800.f);
            }

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            proj = glm::ortho(0.f, (float)display_w, 0.f, (float)display_h, -1.0f, 1.0f);
            renderer.Clear();
            renderer.Draw(va, ib, shader);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}