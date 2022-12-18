#include "Menu.h"
#include "SimulatedAnnealing.h"

Menu::Menu() {
    option = ' ';
    numberOfVertices = 0;
    filename = "Brak wczytanego pliku";
    filenameIni = "settings.ini";
}

Menu::~Menu() {
    if (numberOfVertices != 0) {
        for (int i = 0; i < numberOfVertices; i++)
            delete vertices[i];
        delete[] vertices;
        vertices = NULL;
        numberOfVertices = 0;
    }
}

int Menu::LoadFile(std::string &filename) {
    std::string fileExt = filename.substr(filename.find_last_of(".") + 1);
    if (fileExt != "txt" && fileExt != "tsp") {
        filename += ".txt";
    }
    fileExt = filename.substr(filename.find_last_of(".") + 1);

    if (fileExt == "txt") {
        return Menu::LoadFileTXT(filename);

    }

    if (fileExt == "tsp") {
        return Menu::LoadFileTSP(filename);
    }

    if (fileExt == "atsp") {
        return Menu::LoadFileATSP(filename);
    }

    return 0;
}

int Menu::LoadFileTXT(std::string &filename) {
    std::ifstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        this->filename = "Brak wczytanego pliku";
        std::cout << "\nNie mozna wczytac pliku txt! Nie istnieje\n";
        return 0;
    }

    if (numberOfVertices != 0) {
        for (int i = 0; i < numberOfVertices; i++)
            delete vertices[i];
        delete[] vertices;
        vertices = NULL;
        numberOfVertices = 0;
    }

    this->filename = filename;
    File >> numberOfVertices;
    vertices = new int *[numberOfVertices];
    int line = 0;

    for (int i = 0; i < numberOfVertices; i++)
        vertices[i] = new int[numberOfVertices];

    while (!File.eof()) {
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                File >> vertices[i][j];
            }

            line++;
        }
    }
    File.close();
    return 1;
}

int Menu::LoadFileTSP(std::string &filename) {
    std::ifstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        this->filename = "Brak wczytanego pliku";
        std::cout << "\nNie mozna wczytac pliku tsp!\n";
        return 0;
    }
    this->filename = filename;

    if (numberOfVertices != 0) {
        for (int i = 0; i < numberOfVertices; i++)
            delete vertices[i];
        delete[] vertices;
        vertices = NULL;
        numberOfVertices = 0;
    }
    std::string line;
    while (!File.eof()) {
        getline(File, line);
        {
            if (!line.compare(0, 6, "TYPE: ")) {
                if (!line.compare(7, 3, "TSP")) {
                    std::cout << "\n To nie jest plik TSP\n";
                    return 1;
                }

            } else if (!line.compare(0, 7, "TYPE : ")) {
                if (!line.compare(8, 3, "TSP")) {
                    std::cout << "\n To nie jest plik TSP\n";
                    return 1;
                }
            }

            if (!line.compare(0, 11, "DIMENSION: ")) {
                std::string number = line.substr(11, 100);
                numberOfVertices = stoi(number);
            } else if (!line.compare(0, 12, "DIMENSION : ")) {
                std::string number = line.substr(12, 100);
                numberOfVertices = stoi(number);
            }


            if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: ")) {
                if (!line.compare(19, 8, "EXPLICIT")) {
                    std::cout << "\n Nieobslugiwany typ danych\n";
                    return 1;
                }
            } else if (!line.compare(0, 19, "EDGE_WEIGHT_TYPE : ")) {
                if (!line.compare(20, 8, "EXPLICIT")) {
                    std::cout << "\n Nieobslugiwany typ danych\n";
                    return 1;
                }
            }


            if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) {
                if (!line.compare(20, 11, "FULL_MATRIX")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < numberOfVertices; j++)
                                for (int k = 0; k < numberOfVertices; k++)
                                    File >> vertices[j][k];
                        }
                    }
                } else if (!line.compare(20, 14, "LOWER_DIAG_ROW")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    int line = 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < numberOfVertices; j++) {
                                for (int k = 0; k < line; k++) {
                                    File >> vertices[j][k];
                                    if (j != k)
                                        vertices[k][j] = vertices[j][k];
                                }
                                if (line < numberOfVertices)
                                    line++;
                            }

                        }
                    }
                } else if (!line.compare(20, 14, "UPPER_DIAG_ROW")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    int line = numberOfVertices - 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = numberOfVertices - 1; j >= 0; j--) {
                                for (int k = line; k >= 0; k--) {
                                    File >> vertices[j][k];
                                    if (j != k)
                                        vertices[k][j] = vertices[j][k];
                                }
                                if (line >= 0)
                                    line--;
                            }
                        }
                    }
                } else {
                    std::cout << "\nNieobslugiwany format macierzy\n";
                    return 1;
                }
            } else if (!line.compare(0, 21, "EDGE_WEIGHT_FORMAT : ")) {
                if (!line.compare(21, 11, "FULL_MATRIX")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < numberOfVertices; j++)
                                for (int k = 0; k < numberOfVertices; k++)
                                    File >> vertices[j][k];
                        }
                    }
                } else if (!line.compare(21, 14, "LOWER_DIAG_ROW")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    int line = 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < numberOfVertices; j++) {
                                for (int k = 0; k < line; k++) {
                                    File >> vertices[j][k];
                                    if (j != k)
                                        vertices[k][j] = vertices[j][k];
                                }
                                if (line < numberOfVertices)
                                    line++;
                            }

                        }
                    }
                } else if (!line.compare(21, 14, "UPPER_DIAG_ROW")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    int line = numberOfVertices - 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = numberOfVertices - 1; j >= 0; j--) {
                                for (int k = line; k >= 0; k--) {
                                    File >> vertices[j][k];
                                    if (j != k)
                                        vertices[k][j] = vertices[j][k];
                                }
                                if (line >= 0)
                                    line--;
                            }
                        }
                    }
                } else {
                    std::cout << "\nNieobslugiwany format macierzy\n";
                    return 1;
                }
            }
        }

    }
    File.close();
    return 1;
}

int Menu::LoadFileATSP(std::string &filename) {
    std::ifstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        this->filename = "Brak wczytanego pliku";
        std::cout << "\nNie mozna wczytac pliku atsp!\n";
        return 0;
    }
    this->filename = filename;

    if (numberOfVertices != 0) {
        for (int i = 0; i < numberOfVertices; i++)
            delete vertices[i];
        delete[] vertices;
        vertices = NULL;
        numberOfVertices = 0;
    }

    std::string line;
    while (!File.eof()) {
        getline(File, line);
        {
            if (!line.compare(0, 6, "TYPE: "))
                if (!line.compare(7, 4, "ATSP")) {
                    std::cout << "\n To nie jest plik ATSP\n";
                    return 1;
                }

            if (!line.compare(0, 11, "DIMENSION: ")) {
                std::string number = line.substr(11, 100);
                numberOfVertices = stoi(number);
            }
            if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: "))
                if (!line.compare(19, 8, "EXPLICIT")) {
                    std::cout << "\n Nieobslugiwany typ danych\n";
                    return 1;
                }
            if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) {
                if (!line.compare(20, 11, "FULL_MATRIX")) {

                    vertices = new int *[numberOfVertices];
                    for (int i = 0; i < numberOfVertices; i++)
                        vertices[i] = new int[numberOfVertices];

                    std::string temp;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < numberOfVertices; j++)
                                for (int k = 0; k < numberOfVertices; k++)
                                    File >> vertices[j][k];
                        }
                    }
                } else {
                    std::cout << "\nNieobslugiwany format macierzy\n";
                    return 1;
                }
            }
        }

    }
    File.close();
    return 1;
}

void Menu::LoadINI(std::string &filenameIni) {
    std::string fileExt = filenameIni.substr(filenameIni.find_last_of(".") + 1);
    if (fileExt != "ini") {
        return;
    }
    std::ifstream File;
    File.open(filenameIni, std::ios::in);
    if (!File.good()) {
        this->filenameIni = "Brak wczytanego pliku";
        std::cout << "\nNie mozna wczytac pliku ini! Nie istnieje\n";
        return;
    }

    std::cout << "\n\nWczytano plik ini\n\n";

    std::string inputLine;
    std::ofstream output;
    getline(File, outputfile);
    output.open(outputfile, std::ios::app);

    SimulatedAnnealing dp;

    while (!File.eof()) {
        File >> inputLine;
        filename = inputLine;
        output << inputLine<< " ";
        File >> inputLine;
        numberOfTries = stoi(inputLine);
        output << inputLine << " ";
        File >> inputLine;
        output << inputLine << "\n";

        output.close();
        int loaded = LoadFile(filename);
        if (loaded != 0){
            dp.autotests(numberOfVertices, vertices, numberOfTries, outputfile);
        }
        std::cout<< "\nAutotests finished\n";
    }
    File.close();
}

void Menu::initializeINI(std::string &filenameIni) {
    std::string fileExt = filenameIni.substr(filenameIni.find_last_of(".") + 1);
    if (fileExt != "ini") {
        return;
    }
    std::ifstream File;
    File.open(filenameIni, std::ios::in);
    if (!File.good()) {
        this->filenameIni = "Brak wczytanego pliku";
        std::cout << "\nNie mozna wczytac pliku ini! Nie istnieje\n";
        return;
    }

    std::cout << "\n\nWczytano plik ini\n\n";

    std::string inputLine;
    SimulatedAnnealing sa;

    File >> inputLine;
    coolingMethod = stoi(inputLine);
    File >> inputLine;
    swapMethod = stoi(inputLine);
    File >> inputLine;
    temperature = stod(inputLine);
    File >> inputLine;
    coolingCoefficient = stod(inputLine);
    File >> inputLine;
    eraLength = stod(inputLine);

    sa.initializeFromIni(coolingMethod, swapMethod, temperature, coolingCoefficient, eraLength);
    sa.run(numberOfVertices, vertices, outputfile);
}

int Menu::ShowVertices() {
    std::cout << "\nLiczba wierzcholkow: " << numberOfVertices << std::endl << std::endl;
    if (numberOfVertices > 50) {
        std::cout << "Za duza liczba wierzcholkow\n";
        return 0;
    } else if (numberOfVertices > 0)
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                std::cout << vertices[i][j] << "\t";
            }
            std::cout << std::endl << std::endl;
        }
    return 0;
}

void Menu::ShowMenu() {
    SimulatedAnnealing dp;
    while (option != '9') {
        if (option == ' ')
            std::cout
                    << "\n---------------------------\nAlgorytm Simulated Annealing\n\n1. Wczytaj plik\n2. Pokaz wierzcholki grafu\n3. Przeprowadz test SA\n4. Wczytaj plik ini i przeprowadz testy\n9. Wyjscie\n";
        option = _getch();
        std::string file;

        switch (option) {
            case '1':
                std::cout << "\nWpisz nazwe pliku:\n";
                std::cin >> file;
                LoadFile(file);
                std::cout << "\nWczytano\n";
                break;
            case '2':
                ShowVertices();
                break;
            case '3':
                initializeINI(filenameIni);
                //dp.run(numberOfVertices, vertices, outputfile);
                break;
            case '4':
                LoadINI(filenameIni);
                break;
        }
    }
}
