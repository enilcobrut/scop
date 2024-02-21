
#include <GL/glew.h>
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



//     Vec3 dimensions = Vec3::calculateDimensions(vertices);
//     Vec3 objectCenter = Vec3::calculateObjectCenter(vertices);
//     float maxDimension = std::max({dimensions.x, dimensions.y, dimensions.z});
//     float desiredMaxSize = 2.0f; // Taille maximale souhaitée dans la scène
//     float scaleFactor = (maxDimension > desiredMaxSize) ? (desiredMaxSize / maxDimension) : 1.0f;

//     Mat4 model = Mat4(); // Matrice identité
//     model = Mat4::translate(-objectCenter) * model; // Centre l'objet
//    // model = Mat4::scale(Vec3(scaleFactor, scaleFactor, scaleFactor)) * model; // Ajuste la taille

//     Mat4 view = Mat4(); // Matrice identité
//     Mat4 projection = Mat4(); // Matrice identité
//     float angle = radians(0.0f); // Convertir en radians
//     //model = Mat4::scale(Vec3(scaleFactor, scaleFactor, scaleFactor));
//     printVerticesAndCenter(vertices);
//     // Rotation
//     model = Mat4::rotate(angle, Vec3(1.0f, 1.0f, 1.0f)) * model;

//     // Translation
//     float cameraDistance = 5.0f;

//     // Calculez la position de la caméra basée sur le centre de l'objet
//     float cameraHeightOffset = 0.0f; // Ajustez cette valeur si nécessaire
//         Vec3 cameraPosition = Vec3(objectCenter.x, objectCenter.y + cameraHeightOffset, objectCenter.z - cameraDistance);

//     view = Mat4::lookAt(cameraPosition, objectCenter, Vec3(0.0f, 1.0f, 0.0f));

//     view = Mat4::translate(Vec3(0.0f, 0.0f, -3.0f)) * view;

//     // Perspective
//     projection = Mat4::perspective(radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);

//     glUseProgram(shaderProgram);
//     unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
//     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//     unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
//     glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.value_ptr());

    Vec3 dimensions = Vec3::calculateDimensions(vertices);
    Vec3 objectCenter = Vec3::calculateObjectCenter(vertices);
    float maxDimension = std::max({dimensions.x, dimensions.y, dimensions.z});

    // Déterminer la distance de la caméra en fonction de la plus grande dimension de l'objet
    float cameraDistance = maxDimension * 2.0f; // Vous pouvez ajuster le multiplicateur selon les besoins
    Mat4 model = Mat4(); // Matrice identité
    model = Mat4::translate(-objectCenter) * model; // Centre l'objet

        Mat4 view = Mat4(); // Matrice identité
    Mat4 projection = Mat4(); // Matrice identité

    // Position de la caméra
    Vec3 cameraPosition = Vec3(objectCenter.x, objectCenter.y, objectCenter.z - cameraDistance);
    Vec3 cameraOffset = Vec3(0.0f, 0.0f, cameraDistance); // Décalage derrière l'objet
    cameraPosition = objectCenter - cameraOffset;

    view = Mat4::lookAt(cameraPosition, objectCenter, Vec3(0.0f, 1.0f, 0.0f));

    // Configuration de la perspective
    projection = Mat4::perspective(radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.value_ptr());


    // float rotationSpeed = 0.03f;
    // float angleX = 0.0f; // Angle de rotation autour de l'axe X
    // float angleY = 0.0f; // Angle de rotation autour de l'axe Y

    

//         while (!glfwWindowShouldClose(window)) {
//                 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//             //float cameraHeightOffset = 0.0f; // Ou ajustez cette valeur en fonction de vos besoins
//                  maxDimension = std::max({dimensions.x, dimensions.y, dimensions.z});
//                  cameraDistance = maxDimension * 2.0f; // Ajustez ce facteur au besoin
//                     //std::cout << "Camera Distance: " << cameraDistance << std::endl;
//                 std::cout << "Object Center: X = " << objectCenter.x 
//               << ", Y = " << objectCenter.y 
//               << ", Z = " << objectCenter.z << std::endl;

//             // Recalculer la position de la caméra
//             cameraPosition = objectCenter + Vec3(0.0f, 0.0f, 10.1f);
//             glm::mat4 view = glm::lookAt(glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z),
//                                         glm::vec3(objectCenter.x, objectCenter.y, objectCenter.z),
//                                         glm::vec3(0.0f, 1.0f, 0.0f));

//             // Configuration du modèle
//             model = Mat4::translate(-objectCenter) * Mat4(); // Centre l'objet
//             // Commentez la ligne ci-dessous si l'objet est suffisamment visible sans mise à l'échelle
//             // model = Mat4::scale(Vec3(scaleFactor, scaleFactor, scaleFactor)) * model;
//             model = Mat4::translate(objectCenter) * model; // Déplace l'objet au centre pour la rotation
//             model = Mat4::rotate(angleX, Vec3(1.0f, 0.0f, 0.0f)) * model; // Rotation X
//             model = Mat4::rotate(angleY, Vec3(0.0f, 1.0f, 0.0f)) * model; // Rotation Y
//             model = Mat4::translate(-objectCenter) * model; // Remet l'objet à sa position d'origine

//             if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//         angleX -= rotationSpeed; // Rotation vers le haut
//             }
//             if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//                 angleX += rotationSpeed; // Rotation vers le bas
//             }
//             if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//                 angleY += rotationSpeed; // Rotation vers la droite
//             }
//             if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//                 angleY -= rotationSpeed; // Rotation vers la gauche
//             }
//             // Mettez à jour les transformations si nécessaire

//             // Par exemple, pour faire tourner le cube autour de son axe
//                 //    model = Mat4::rotate(angleX, Vec3(1.0f, 0.0f, 0.0f)) * model;
//                 // model = Mat4::rotate(angleY, Vec3(0.0f, 1.0f, 0.0f)) * model;

//             // Utilisez le programme shader
//             glUseProgram(shaderProgram);

//             // Passer les matrices au shader
//             glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
// glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//             glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.value_ptr());


//             // Dessinez le cube
//             glBindVertexArray(VAO);
//             glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

//             // Échangez les tampons et interrogez les événements IO
//             glfwSwapBuffers(window);
//             glfwPollEvents();
            //         }
          
//         while (!glfwWindowShouldClose(window)) {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     // Utiliser GLM pour calculer la position de la caméra et les matrices
//     glm::vec3 dimensions = calculateDimensions(vertices); // Assurez-vous que cette fonction retourne un glm::vec3
//     glm::vec3 objectCenter = calculateObjectCenter(vertices); // De même pour cette fonction
//     float maxDimension = glm::max(glm::max(dimensions.x, dimensions.y), dimensions.z);
//     float cameraDistance = maxDimension * 2.0f;

//     glm::vec3 cameraPosition = objectCenter + glm::vec3(0.0f, 0.0f, cameraDistance);
//     glm::mat4 view = glm::lookAt(cameraPosition, objectCenter, glm::vec3(0.0f, 1.0f, 0.0f));
//     glm::mat4 model = glm::mat4(1.0f); // Matrice identité
//     glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);

//     // Utiliser les matrices GLM avec OpenGL
//     glUseProgram(shaderProgram);
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//     glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

//     // Dessiner l'objet
//     glBindVertexArray(VAO);
//     glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

//     glfwSwapBuffers(window);
//     glfwPollEvents();
// }



float angleX = 0.0f; // Angle de rotation autour de l'axe X
float angleY = 0.0f; // Angle de rotation autour de l'axe Y


while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vec3 dimensions =Vec3::calculateDimensions(vertices); // Utilisez votre fonction modifiée
    Vec3 objectCenter = Vec3::calculateObjectCenter(vertices); // De même pour cette fonction
    float maxDimension = std::max({dimensions.x, dimensions.y, dimensions.z});
    float cameraDistance = maxDimension * 2.0f;

    Vec3 cameraPosition = objectCenter + Vec3(0.0f, 0.0f, cameraDistance);
    Mat4 view = Mat4::lookAt(cameraPosition, objectCenter, Vec3(0.0f, 1.0f, 0.0f));
    Mat4 model = Mat4(); // Votre méthode pour créer une matrice identité
    Mat4 projection = Mat4::perspective(Mat4::radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);
   //glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);

    // Utilisez vos matrices avec OpenGL
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.value_ptr());
   //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
