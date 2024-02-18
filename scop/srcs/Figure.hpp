#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <math.h>
#include <map>
#include "Point.hpp"

class Figure {
    public :
        Figure(std::string filename);
        ~Figure();

        // Parce
        void    parcefile();
        void    parceLine(const std::string &line);
        void    parceVertexLine(const std::vector<std::string> &splitLine);
        void    parceVnLine(const std::vector<std::string> &splitLine);
        void    parceVtLine(const std::vector<std::string> &splitLine);
        void    parceFaceLine(const std::vector<std::string> &splitLine);
        void    parceFaceValue(const std::vector<std::string> &splitValue);  

        // Parce Face

        int onlyVertex(const std::string &splitValue);
        int vertexAndVt(const std::vector<std::string> &splitValue);
        int vertexAndVtAndVn(const std::vector<std::string> &splitValue);

        std::map<int, std::array<float, 3>> getListCoordVertex() const;
        std::map<int, std::array<float, 3>> getListCoordVn() const;
        std::map<int, std::array<float, 2>> getListCoordVt() const;

        void    setListCoordVertex(const std::map<int, std::array<float, 3>>& newValues);
        void    setListCoordVn(const std::map<int, std::array<float, 3>>& newValues);
        void    setListCoordVt(const std::map<int, std::array<float, 2>>& newValues);
        int getNorme(const std::array<float, 3> &coordVertex);

        // utils 
        void    printListCoordVertex() const;
        void    printListCoordVn() const;
        void    printListCoordVt() const;
        void    printFace() const;

    protected :
        std::string filename;
        std::map<int, std::array<float, 3>> listCoordVertex;
        std::map<int, std::array<float, 3>> listCoordVn;
        std::map<int, std::array<float, 2>> listCoordVt;
        std::vector<std::vector<Point>> listFace;
};
