
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <cstring>
#include <filesystem>
#include "Figure.hpp"
#include <glm.hpp>

#include "/home/cjunker/glm/gtc/matrix_transform.hpp"
#include "/home/cjunker/glm/gtc/type_ptr.hpp"
#include "Vec3.hpp"
#include "Mat4.hpp"

#include "scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}  

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Please select a .obj file to run this program." << std::endl;
        return -1;
    }

    std::filesystem::path filePath(argv[1]);
    if (!std::filesystem::is_regular_file(filePath) || filePath.extension() != ".obj") {
        std::cout << "The file needs to be a valid .obj file." << std::endl;
        return -1;
    }

    Figure myFigure(filePath.string());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // glfwWindowHint(GLFW_SAMPLES, 256);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  


    std::vector<float> vertices =  myFigure.getVertexNormalList();

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
