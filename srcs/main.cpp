
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <filesystem>
#include "Figure.hpp"
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "/home/cjunker/glm/gtc/matrix_transform.hpp"
#include "/home/cjunker/glm/gtc/type_ptr.hpp"
#include "Cam.hpp"

void error_callback(int error, const char* description) {
    std::cout << "error: " << error << std::endl;
    std::cerr << "GLFW Error: " << description << std::endl;
}

     //   Figure myFigure(filePath.string());

        // std::cout << "\n====== map list vertex :  ======\n" << std::endl;

        // myFigure.printListCoordVertex();

        // std::cout << "\n====== map list Vn :  ======\n" << std::endl;

        // myFigure.printListCoordVn();

        // std::cout << "\n====== map list Vt :  ======\n" << std::endl;

        // myFigure.printListCoordVt();
        // std::cout << "\n====== Face et Points :  ======\n" << std::endl;

        // myFigure.printFace();
     
glm::vec3 calculateObjectCenter(const std::vector<float>& vertices) {
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    int count = 0;

    // Parcourir les sommets et accumuler leurs coordonnées
    for (size_t i = 0; i < vertices.size(); i += 6) { // Incrémentation de 6 pour passer au sommet suivant
        center.x += vertices[i];
        center.y += vertices[i + 1];
        center.z += vertices[i + 2];
        count++;
    }

    // Diviser par le nombre total de sommets pour obtenir la moyenne
    if (count > 0) {
        center.x /= count;
        center.y /= count;
        center.z /= count;
    }

    return center;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)xoffset;
    Cam* cam = static_cast<Cam*>(glfwGetWindowUserPointer(window));
    float newDistance = cam->getDistance() - static_cast<float>(yoffset) * 0.1f;
    // Assurez-vous que la nouvelle distance reste dans des limites raisonnables
    newDistance = std::max(newDistance, 1.0f);
    newDistance = std::min(newDistance, 10.0f);
    cam->setDistance(newDistance);
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
    Cam camera(3.0f); // Initialiser la caméra avec une distance de 3.0f
    glfwSetWindowUserPointer(window, &camera);
    glfwSetScrollCallback(window, scroll_callback);

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
    Normal = mat3(model) * aNormal; // Transforme les normales avec la matrice de modèle
}
)glsl";


    const char* fragmentShaderSource = R"glsl(
    #version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(2.0f, 4.0f, 5.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Éclairage diffus
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ajouter un éclairage ambiant
    vec3 ambient = 0.2 * lightColor;

    // Combiner l'éclairage ambiant et diffus
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
    
    //float distanceCamera = 3.0f; // Distance initiale de la caméra
    glm::vec3 objectCenter = calculateObjectCenter(vertices); // Calculez le centre de l'objet


    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    float angle = 0.0f; // Angle de rotation en degrés


    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(60.0f), // Augmenter le FOV pour "zoomer arrière"
                              (float)800 / (float)800, 0.1f, 100.0f);
    glUseProgram(shaderProgram);


    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

    float rotationSpeed = 1.0f;
     float angleX = 0.0f; // Angle initial de rotation autour de l'axe X
        float angleY = 0.0f; // Angle initial de rotation autour de l'axe Y

            

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            model = glm::mat4(1.0f);

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        angleX -= rotationSpeed; // Rotation vers le haut
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                angleX += rotationSpeed; // Rotation vers le bas
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                angleY += rotationSpeed; // Rotation vers la droite
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                angleY -= rotationSpeed; // Rotation vers la gauche
            }
            angleX = fmod(angleX, 360.0f);
            angleY = fmod(angleY, 360.0f);
            // Mettez à jour les transformations si nécessaire
                float currentDistance = camera.getDistance();

                  glm::vec3 cameraPos = objectCenter + glm::vec3(currentDistance * sin(glm::radians(angleY)) * cos(glm::radians(angleX)),
                                                   currentDistance * sin(glm::radians(angleX)),
                                                   currentDistance * cos(glm::radians(angleY)) * cos(glm::radians(angleX)));
    view = glm::lookAt(cameraPos, objectCenter, glm::vec3(0.0f, 1.0f, 0.0f));


            // Utilisez le programme shader
            glUseProgram(shaderProgram);

            // Passer les matrices au shader
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Dessinez le cube
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

            // Échangez les tampons et interrogez les événements IO
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
