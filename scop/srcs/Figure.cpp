#include "Figure.hpp"
#include "scop.hpp"
#include <stdexcept>
#include <cfloat>

Figure::Figure(std::string filename) : filename(filename) {
    parcefile();
}

void Figure::printListCoordVertex() const {
    for (const auto& pair : listCoordVertex) {
        std::cout << "Index: " << pair.first << " - Values: ";
        for (float val : pair.second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void Figure::printListCoordVn() const {
    for (const auto& pair : listCoordVn) {
        std::cout << "Index: " << pair.first << " - Values: ";
        for (float val : pair.second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void Figure::printListCoordVt() const {
    for (const auto& pair : listCoordVt) {
        std::cout << "Index: " << pair.first << " - Values: ";
        for (float val : pair.second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}



void Figure::parceVertexLine(const std::vector<std::string> &splitLine) {
    static int index = 0;
    std::array<float, 3> coordVertex;
    
    if (splitLine.size() != 4) {
        throw std::runtime_error("Error : a Vertex needs 3 coordinates : z y z");
    }
    for (size_t i = 1; i < 4; ++i) {
        float value = std::stof(splitLine[i]);
        if (value > FLT_MAX) {
            throw std::runtime_error("Error : unexepted value");
        }
        coordVertex[i - 1] = value;  
    }

    this->listCoordVertex[index] = coordVertex;
    index += 1;
}

void Figure::parceVnLine(const std::vector<std::string> &splitLine) {
    static int index = 0;
    std::array<float, 3> coordVn;
    if (splitLine.size() != 4) {
        throw std::runtime_error("Error : a Vertex Normal needs 3 coordinates : z y z");
    }
    for (size_t i = 1; i < 4; ++i) {
        float value = std::stof(splitLine[i]);
        if (value > FLT_MAX) {
            throw std::runtime_error("Error : unexepted value");
        }
        coordVn[i - 1] = value;  
    }
   // std::cout << "index Vn : " << index << std::endl;
    this->listCoordVn[index] = coordVn;
    index += 1;
}





void Figure::parceVtLine(const std::vector<std::string> &splitLine) {
    static int index = 0;
    std::array<float, 2> coordVt;
    if (splitLine.size() != 3) {
        throw std::runtime_error("Error : a Vertex Texture needs 2 coordinates : u v");
    }
    for (size_t i = 1; i < 3; ++i) {
        float value = std::stof(splitLine[i]);
        if (value > FLT_MAX) {
            throw std::runtime_error("Error : unexepted value");
        }
        coordVt[i - 1] = value;  
    }
    //std::cout << "index Vt : " << index << std::endl;
    this->listCoordVt[index] = coordVt;
    index += 1;
}

int Figure::onlyVertex(const std::string &splitValue) {
    int vertexIndex = std::stoi(splitValue) - 1;
    if (listCoordVertex.find(vertexIndex) == listCoordVertex.end()) {
        throw std::runtime_error("Error: the index is not in listCoordVertex");
    }
    return vertexIndex;

}

int Figure::vertexAndVt(const std::vector<std::string> &splitValue){
    int vtIndex = std::stoi(splitValue[1]) - 1;
    if (listCoordVt.find(vtIndex) == listCoordVt.end()) {
        throw std::runtime_error("Error: the index is not in listCoordVt");
    }

    return vtIndex;
}

int Figure::vertexAndVtAndVn(const std::vector<std::string> &splitValue){
    std::cout << "before  crash : " << splitValue[2] << std::endl;
    int vnIndex = std::stoi(splitValue[2]) - 1;
    std::cout << "vnIndex" << std::endl;
    if (listCoordVn.find(vnIndex) == listCoordVn.end()) {
        throw std::runtime_error("Error: the index is not in listCoordVn");
    }
    return vnIndex;
}
   

void Figure::parceFaceValue(const std::vector<std::string> &splitValue) {
    // Affichage du contenu de splitValue
   // std::cout << "splitValue: ";
    // for (const auto& val : splitValue) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;

    //std::cout << "SIZE : " << splitValue.size() << std::endl;
 
        int vIndex = -1, vtIndex = -1, vnIndex = -1;
        // Vérification et traitement de chaque partie de faceElements
        if (splitValue.size() == 1) {
          //  std::cout <<"COUCOU" << std::endl;
            vIndex = onlyVertex(splitValue[0]);
        }
        if (splitValue.size() == 2) {
          //    std::cout <<"COUCOU2" << std::endl;
            vIndex = onlyVertex(splitValue[0]);
            vtIndex = vertexAndVt(splitValue);
        }
        if (splitValue.size() == 3) {
            vIndex = onlyVertex(splitValue[0]);
            std::cout << "\""<< splitValue[0] << "\""<< std::endl;
            std::cout << "\""<< splitValue[1] << "\""<< std::endl;
            std::cout << "\""<< splitValue[2] << "\""<< std::endl;
            if (splitValue[1] != "")
                vtIndex = vertexAndVt(splitValue);
            
            std::cout <<"COUCOU3" << std::endl;
            vnIndex = vertexAndVtAndVn(splitValue);
        }


        //std::cout << "vnIndex : " << vnIndex << std::endl;
        auto &&indexV = this->listCoordVertex.at(vIndex);
        Point p(vIndex, vtIndex, vnIndex, indexV);
        this->vecPoint.push_back(p);

}

void Figure::triangusation() {
    std::vector<std::vector<Point>> newFaces;

    // Triangulation pour les quadrilatères
    if (vecPoint.size() == 4) {
        newFaces.push_back({vecPoint[0], vecPoint[1], vecPoint[2]});
        newFaces.push_back({vecPoint[0], vecPoint[2], vecPoint[3]});
    } else {
        // Triangulation pour les polygones avec plus de 4 sommets
        // Crée un "ventilateur" de triangles à partir du premier sommet
        for (size_t i = 1; i < vecPoint.size() - 1; ++i) {
            newFaces.push_back({vecPoint[0], vecPoint[i], vecPoint[i + 1]});
        }
    }

    // Ajoute les nouvelles faces triangulées à listFace
    for (const auto& face : newFaces) {
        this->listFace.push_back(face);
    }
}

void Figure::parceFaceLine(const std::vector<std::string> &splitLine) {
    vecPoint.clear();

 // case if listCoordVt not empty
 // case if listCoordVn not empty
    for (const std::string& value : splitLine) {
            if (value == "f") continue;
                std::vector<std::string> splitValue = split1(value, '/');
            parceFaceValue(splitValue);
         //   std::cout << std::endl;
        }
        if (vecPoint.size() >= 3) {
            triangusation();
        }
        else
            this->listFace.push_back(vecPoint);
}


void Figure::parceLine(const std::string &line) {
  //  std::cout << line << std::endl;
    std::vector<std::string> splitLine = split(line, ' ');

    if (splitLine[0] == "v") {
        parceVertexLine(splitLine);
    }

    else if (splitLine[0] == "vn") {
        parceVnLine(splitLine);
    }

    else if (splitLine[0] == "vt") {
        parceVtLine(splitLine);
    }

    else if (splitLine[0] == "f") {
        parceFaceLine(splitLine);
    }
}

void Figure::parcefile() {
    std::ifstream fileStream(filename);

    if (!fileStream.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fileStream, line)) {
        if ((line[0] == 'v' && line[1] == ' ') || (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') || (line[0] == 'v' && line[1] == 't' && line[2] == ' ') || (line[0] == 'f' && line[1] == ' ')) {   
            parceLine(line);
        }
            
    }

    fileStream.close();
}
std::map<int, std::array<float, 3>> Figure::getListCoordVertex() const {
    return listCoordVertex;
}

std::map<int, std::array<float, 3>> Figure::getListCoordVn() const {
    return listCoordVn;
}

std::map<int, std::array<float, 2>> Figure::getListCoordVt() const {
    return listCoordVt;
}

void Figure::setListCoordVertex(const std::map<int, std::array<float, 3>>& newValues) {
    listCoordVertex = newValues;
}

void Figure::setListCoordVn(const std::map<int, std::array<float, 3>>& newValues) {
    listCoordVn = newValues;
}

void Figure::setListCoordVt(const std::map<int, std::array<float, 2>>& newValues) {
    listCoordVt = newValues;
}
Figure::~Figure() {

}


void Figure::printFace() const {
    int faceNum = 0;
    for (const auto& face : listFace) {
        std::cout << CYAN<< "Face " << faceNum++ << ":"  << std::endl;
        std::cout << "Face size : " << face.size() << ":" << RESET << std::endl;
        for (const auto& point : face) {

            std::cout << "vertex index : " << point.getIndexV() << std::endl;;
            auto &&indexV = this->listCoordVertex.at(point.getIndexV());
            std::cout << "coord : x: "<< indexV[0] << " y: " << indexV[1] << " z: " << indexV[2] << std::endl;


            if (point.getIndexVt() != -1) {
                auto &&indexVt = this->listCoordVt.at(point.getIndexVt());
                std::cout << "vt index " << point.getIndexVt() << std::endl;
                 std::cout << "coord : u: "<< indexVt[0] << " v: " << indexV[1] << std::endl;
            }
            if (point.getIndexVn() != -1) {
                std::cout << "vn index " << point.getIndexVn() << std::endl;
                auto &&indexVn = this->listCoordVn.at(point.getIndexVn());
                std::cout << "coord vn : x: "<< indexVn[0] << " y: " << indexVn[1] << " z: " << indexV[2] << std::endl;
            }
            else if (point.getIndexVn() == -1) {
                auto &&indexVn = point.getNorm();
                std::cout << "coord vn : x: "<< indexVn[0] << " y: " << indexVn[1] << " z: " << indexV[2] << std::endl;

            }
            std::cout << std::endl;
        }
    }
}



std::vector<float> Figure::getVertexNormalList() {
    std::vector<float> res;
    for (const auto& face : listFace) {
         for (const auto& point : face) {
              auto &&indexV = this->listCoordVertex.at(point.getIndexV());
              res.push_back(indexV[0]);
              res.push_back(indexV[1]);
              res.push_back(indexV[2]);
            if (point.getIndexVn() != -1) {
                auto &&indexVn = this->listCoordVn.at(point.getIndexVn());
                res.push_back(indexVn[0]);
                res.push_back(indexVn[1]);
                res.push_back(indexVn[2]);
            }
            else if (point.getIndexVn() == -1) {
                auto &&indexVn = point.getNorm();
                res.push_back(indexVn[0]);
                res.push_back(indexVn[1]);
                res.push_back(indexVn[2]);
            }

         }

    }
    return res;

}