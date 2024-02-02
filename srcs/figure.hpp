#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <GLFW/glfw3.h>
#include <array>

struct FaceVertex {
    GLuint vertexIndex;
    std::optional<GLuint> textureIndex;
    std::optional<GLuint> normalIndex;

    FaceVertex(GLuint vIdx, std::optional<GLuint> tIdx = std::nullopt, std::optional<GLuint> nIdx = std::nullopt)
        : vertexIndex(vIdx), textureIndex(tIdx), normalIndex(nIdx) {}
};

struct Face {
    std::vector<FaceVertex> vertices;
};

class Figure {
public:
    Figure(const std::string& filename) {
        parseObjFile(filename);
    }

    void displayVertices() const {
        for (const auto& vertex : vertices) {
            std::cout << "Vertex: " << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << std::endl;
        }
    }

    void displayFaces() const {
        for (const auto& face : faces) {
            std::cout << "Face: ";
            for (const auto& vertex : face.vertices) {
                std::cout << vertex.vertexIndex << " ";
                if (vertex.textureIndex.has_value()) {
                    std::cout << "/" << vertex.textureIndex.value() << " ";
                }
                if (vertex.normalIndex.has_value()) {
                    std::cout << "/" << vertex.normalIndex.value() << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::array<GLfloat, 3>> vertices; // Stocke les coordonnées des vertices
    std::vector<Face> faces; // Utilise la structure Face pour stocker les faces

    void parseObjFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Erreur lors de l'ouverture du fichier " << filename << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "v") {
                GLfloat x, y, z;
                iss >> x >> y >> z;
                vertices.push_back({x, y, z});
            } else if (prefix == "f") {
                Face face;
                std::string vertex;
                while (iss >> vertex) {
                    std::istringstream vertexStream(vertex);
                    std::string vertexIndex, textureIndex, normalIndex;

                    std::getline(vertexStream, vertexIndex, '/');
                    std::getline(vertexStream, textureIndex, '/');
                    std::getline(vertexStream, normalIndex, '/');

                    GLuint vIdx = std::stoul(vertexIndex) - 1;
                    std::optional<GLuint> tIdx = textureIndex.empty() ? std::nullopt : std::optional<GLuint>(std::stoul(textureIndex) - 1);
                    std::optional<GLuint> nIdx = normalIndex.empty() ? std::nullopt : std::optional<GLuint>(std::stoul(normalIndex) - 1);

                    face.vertices.push_back(FaceVertex(vIdx, tIdx, nIdx));
                }
                faces.push_back(face);
            }
        }

        file.close();
    }
};
