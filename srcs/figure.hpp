#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Figure {
public:
    Figure(const std::string& filename);
    ~Figure();
    
    const std::vector<GLfloat>& getVertices() const { return vertices; }
    const std::vector<GLint>& getFaces() const { return faces; }


    void displayVertices() const {
        const std::vector<GLfloat>& vertices = getVertices();
        for (size_t i = 0; i < vertices.size(); i += 3) {
            std::cout << "Vertex " << i / 3 << ": " << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << std::endl;
        }
    }

    // Méthode pour afficher les indices des faces
    void displayFaces() const {
        const std::vector<GLint>& faces = getFaces();
        for (size_t i = 0; i < faces.size(); i += 3) {
            std::cout << "Face " << i / 3 << ": " << faces[i] << ", " << faces[i + 1] << ", " << faces[i + 2] <<  ", " << faces[i + 3] << std::endl;
        }
    }

private:
    std::vector<GLfloat> vertices; // stocke les coordonnées des sommets
    std::vector<GLint> faces;      // stocke les indices des faces
};

Figure::Figure(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;


    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        iss >> token;
        if (token == "v") {
            GLfloat x, y, z;
            iss >> x >> y >> z;
            std::cout << "Vertex: " << x << " " << y << " " << z << std::endl;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        } else if (token == "f") {
            std::vector<GLint> faceIndices;
            GLint index;
            while (iss >> index) {
               // std::cout << "Read index: " << index << std::endl;
                faceIndices.push_back(index - 1); // Soustraction de 1 pour chaque indice
            }

            // Assurez-vous que la face a trois ou quatre indices
            if (faceIndices.size() == 3 || faceIndices.size() == 4) {
                std::cout << "Face indices: ";
                for (GLint faceIndex : faceIndices) {
                    std::cout << faceIndex << " ";
                }
                std::cout << std::endl;

                // Ajoutez les indices à la liste des faces
                for (GLint faceIndex : faceIndices) {
                    faces.push_back(faceIndex);
                }
            } else {
                std::cerr << "Nombre d'indices de face invalide." << std::endl;
                std::cerr << "Ligne: " << line << std::endl; // Ajout pour afficher la ligne du fichier
                std::cerr << "Indices de face: ";
                for (GLint faceIndex : faceIndices) {
                    std::cerr << faceIndex << " ";
                }
                std::cerr << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }




    file.close();
}


Figure::~Figure() {
}
