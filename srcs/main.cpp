#include <iostream>
#include <GLFW/glfw3.h>
#include <cstring>
#include <filesystem>
#include "figure.hpp"

void error_callback(int error, const char* description) {
    std::cout << "error: " << error << std::endl;
    std::cerr << "GLFW Error: " << description << std::endl;
}




int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "please select a .obj file to run this program" << std::endl;
        return -1;
    }

    std::filesystem::path filePath(argv[1]);




    if (!std::filesystem::is_regular_file(filePath)) {
        std::cout << "the path is not valid" << std::endl;
        return -1;
    }

    if (filePath.extension() != ".obj") {
        std::cout << "Lthe file needs a .obj extension" << std::endl;
        return -1;
    }


    Figure myFigure(filePath.string());

    // myFigure.displayVertices();
    // myFigure.displayFaces();

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Scop", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }



    glfwDestroyWindow(window);



    glfwTerminate();
}
