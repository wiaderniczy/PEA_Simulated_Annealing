#pragma once
#include "includes.h"

class Menu {
    char option;
    int numberOfVertices;
    int** vertices;
    std::string filename;
    std::string filenameIni;
    std::string outputfile;
    int numberOfTries;
    int coolingMethod;
    int swapMethod;
    double temperature;
    double coolingCoefficient;
    double eraLength;


    int LoadFile(std::string &filename);
    int LoadFileTXT(std::string &filename);
    int LoadFileTSP(std::string &filename);
    int LoadFileATSP(std::string &filename);
    void LoadINI(std::string &filenameIni);
    int ShowVertices();
    void initializeINI(std::string &filenameIni);


public:
    Menu();
    ~Menu();
    void ShowMenu();
};