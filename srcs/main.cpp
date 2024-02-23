
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <filesystem>
#include "Figure.hpp"
#include <glm.hpp>

#include "/home/cjunker/glm/gtc/matrix_transform.hpp"
#include "/home/cjunker/glm/gtc/type_ptr.hpp"
#include "/home/cjunker/glm/gtx/quaternion.hpp"

#include "Vec3.hpp"
#include "Mat4.hpp"

#include "scop.hpp"
float radians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

void printVerticesAndCenter(const std::vector<float>& vertices) {
    if (vertices.empty()) {
        std::cout << "Aucun sommet trouvé." << std::endl;
        return;
    }

    Vec3 center(0.0f, 0.0f, 0.0f);
    int count = 0;

    std::cout << "Sommet:" << std::endl;
    for (size_t i = 0; i < vertices.size(); i += 6) {
        // Afficher les coordonnées de chaque sommet
        std::cout << "v " << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << std::endl;

        // Ajouter au centre
        center += Vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        count++;
    }

    if (count > 0) {
        center /= count;
    }

    // Afficher les coordonnées du centre
    std::cout <<RED<< "Centre de l'objet: (" << center.x << ", " << center.y << ", " << center.z << ")" <<RESET<< std::endl;
}


void error_callback(int error, const char* description) {
    std::cout << "error: " << error << std::endl;
    std::cerr << "GLFW Error: " << description << std::endl;
}

glm::vec3 calculateObjectCenter(const std::vector<float>& vertices) {
    glm::vec3 center(0.0f);
    int count = 0;

    for (size_t i = 0; i < vertices.size(); i += 6) { // Supposons 6 valeurs par sommet (x, y, z, nx, ny, nz)
        center += glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        count++;
    }

    if (count > 0) {
        center /= static_cast<float>(count);
    }

    return center;
}

glm::vec3 calculateDimensions(const std::vector<float>& vertices) {
    if (vertices.empty()) return glm::vec3(0.0f);

    float minX = vertices[0], maxX = vertices[0];
    float minY = vertices[1], maxY = vertices[1];
    float minZ = vertices[2], maxZ = vertices[2];

    for (size_t i = 0; i < vertices.size(); i += 6) {
        minX = std::min(minX, vertices[i]);
        maxX = std::max(maxX, vertices[i]);
        minY = std::min(minY, vertices[i + 1]);
        maxY = std::max(maxY, vertices[i + 1]);
        minZ = std::min(minZ, vertices[i + 2]);
        maxZ = std::max(maxZ, vertices[i + 2]);
    }

    return glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
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

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 256);
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    std::vector<float> vertices =  myFigure.getVertexNormalList();

    const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 Normal;
    out vec3 FragPos;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        FragPos = vec3(model * vec4(aPos, 1.0f));
        Normal = mat3(transpose(inverse(model))) * aNormal; // Transforme les normales
    }
    )glsl";

    const char* fragmentShaderSource = R"glsl(
    #version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(4.0f, 8.0f, 5.0f);
    vec3 lightColor = vec3(8.0f, 1.0f, 6.0f);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = 0.2 * lightColor;

    vec3 result = (ambient + diffuse) * vec3(1.0, 0.5, 0.2);

    FragColor = vec4(result, 1.0);
}


    )glsl";

    // Création du vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Vérifiez les erreurs de compilation ici...

    // Création du fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Vérifiez les erreurs de compilation ici...

    // Lien des shaders dans un programme shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Vérifiez les erreurs de lien ici...

    // Supprimez les shaders une fois liés
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(float),vertices.data(), GL_STATIC_DRAW);

    // Attributs de position des sommets
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Notez que cela ne désactive pas le VAO actuellement lié, cela désactive simplement le VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Désactivez le VAO maintenant
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);


        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    float rotationSpeed = 2.5f;


    glm::quat rotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

   while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Gestion des entrées clavier pour la rotation
        
            // Utilisation des matrices GLM avec OpenGL
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Dessiner l'objet
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }




    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
