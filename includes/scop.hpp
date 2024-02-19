#pragma once
#include <vector>
#include <iostream>
#include <math.h>

#define ORANGE						"\033[1;38;2;255;187;54m"
#define VIOLET						"\033[1;38;2;158;46;156m"
#define BLUE						"\033[1;38;2;73;153;254m"
#define RED							"\033[1;38;2;255;0;0m"
#define GREEN						"\033[1;38;2;69;176;26m"
#define YELLOW						"\033[1;38;2;241;223;9m"
#define PINK						"\033[1;38;2;240;170;223m"
#define CYAN						"\033[1;38;2;43;236;195m"
#define JAUNE						"\033[1;38;2;251;255;66"
#define RESET						"\033[0m"

std::vector<std::string> split(const std::string& s, char delimiter);
std::vector<std::string> split1(const std::string& s, char delimiter);
