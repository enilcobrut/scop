#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <GLFW/glfw3.h>
#include <array>
#include <math.h>
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


struct Vector3D {
    GLfloat x, y, z;

    // Constructeur
    Vector3D(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}

    // Soustraction de vecteurs
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    // Produit vectoriel
    Vector3D cross(const Vector3D& v) const {
        return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    // Normalisation du vecteur
    Vector3D normalize() const {
        GLfloat len = sqrt(x * x + y * y + z * z);
        return Vector3D(x / len, y / len, z / len);
    }
};

Vector3D calculateNormal(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) {
    Vector3D edge1 = v2 - v1;
    Vector3D edge2 = v3 - v1;
    return edge1.cross(edge2).normalize();
}


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
        std::vector<std::array<GLfloat, 3>> vertices;
        std::vector<Vector3D> normals; // Ajouter pour stocker les normales calculées
        std::vector<Face> faces;

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
                bool normalIndexEmpty = true;
                while (iss >> vertex) {
                    std::istringstream vertexStream(vertex);
                    std::string vertexIndex, textureIndex, normalIndex;

                    std::getline(vertexStream, vertexIndex, '/');
                    std::getline(vertexStream, textureIndex, '/');
                    std::getline(vertexStream, normalIndex, '/');

                    GLuint vIdx = std::stoul(vertexIndex) - 1;
                    std::optional<GLuint> tIdx = textureIndex.empty() ? std::optional<GLuint>(0) : std::optional<GLuint>(std::stoul(textureIndex) - 1);
                    std::optional<GLuint> nIdx = normalIndex.empty() ? std::nullopt : std::optional<GLuint>(std::stoul(normalIndex) - 1);

                    face.vertices.push_back(FaceVertex(vIdx, tIdx, nIdx));
                    if (!normalIndex.empty()) {
                        normalIndexEmpty = false;
                    }
                }

                // Calculer la normale pour la face si nécessaire
                if (normalIndexEmpty && face.vertices.size() >= 3) {
                    Vector3D v1 = Vector3D(vertices[face.vertices[0].vertexIndex][0], vertices[face.vertices[0].vertexIndex][1], vertices[face.vertices[0].vertexIndex][2]);
                    Vector3D v2 = Vector3D(vertices[face.vertices[1].vertexIndex][0], vertices[face.vertices[1].vertexIndex][1], vertices[face.vertices[1].vertexIndex][2]);
                    Vector3D v3 = Vector3D(vertices[face.vertices[2].vertexIndex][0], vertices[face.vertices[2].vertexIndex][1], vertices[face.vertices[2].vertexIndex][2]);

                    Vector3D normal = calculateNormal(v1, v2, v3);
                    normals.push_back(normal); // Stocker la normale
                    GLuint normalIdx = static_cast<GLuint>(normals.size() - 1);

                    // Assigner cet index à tous les sommets de la face
                    for (auto& vertex : face.vertices) {
                        vertex.normalIndex = normalIdx;
                    }
                }

                faces.push_back(face);
            }
        }

        file.close();
    }

};
