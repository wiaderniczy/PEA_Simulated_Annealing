#include "SimulatedAnnealing.h"

void SimulatedAnnealing::run(int numberOfVertices, int **vertices, std::string outputfile){
    path.resize(numberOfVertices);
    this->numberOfVertices = numberOfVertices;
    this->vertices = vertices;

    for(int i = 0; i < numberOfVertices; i++){
        path.at(i) = i;
    }

    path = ShuffleVertices(path, numberOfVertices);

    int startCost = CalculateCost(path);

    /*
    std::cout<< "\nEra length?\n";
    std::cin>> this->eraLength;
    std::cout<< "\nCooling method:\n1. Boltzmann\n2. Geometrical\n";
    std::cin>> this->coolingMethod;
    std::cout<< "\nNeighborhood swap method:\n1. Swap two\n2. Reverse arc\n";
    std::cin>> this->swapMethod;
    std::cout<< "\nType in the starting temperature\n";
    std::cin>> this->startTemperature;
    if (coolingMethod == 2) {
        std::cout << "\nType in the static value for cooling (0 < x < 1 for geometrical):\n";
        std::cin >> this->coolingCoefficient;
    }
    */
    std::cout<<"Cooling method: "<<this->coolingMethod << std::endl;
    std::cout<<"Swap method: "<<this->swapMethod << std::endl;
    std::cout<<"Start temperature: "<<this->startTemperature << std::endl;
    std::cout<<"Cooling coefficient: "<<this->coolingCoefficient << std::endl;
    std::cout<<"Era length: "<<this->eraLength << std::endl;

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    algorithm(startCost, path);

    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Time elapsed: \n"<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<"ms\n";
    std::cout << "Osiagniety koszt: \n";
    std::cout << minCost << std::endl;
}

void SimulatedAnnealing::algorithm(int startCost, std::vector<int> startPath){
    minCost = 0;
    iteration = 1;
    std::vector<int> tempVector = startPath;
    double temperature = startTemperature;
    int minCurrentCost = startCost;

    int counter = 0;

    while (temperature > 1){
        std::vector<int> shuffled;

        for (int i = 0; i < eraLength;i++) {
            switch (swapMethod) {
                case 1:
                    shuffled = swapTwo(tempVector, this->numberOfVertices);
                    break;
                case 2:
                    shuffled = swapArc(tempVector, this->numberOfVertices);
                    break;
            }

            int currentCost = CalculateCost(shuffled);

            double r = ((float) rand() / RAND_MAX);
            float prob = CalculateProbability(minCurrentCost, currentCost, temperature);

            if (currentCost < minCurrentCost) {
                tempVector = shuffled;
                minCurrentCost = currentCost;
            } else if (prob > r) {
                tempVector = shuffled;
                minCurrentCost = currentCost;
            }
            counter++;
            this->results.push_back(minCurrentCost);
            this->counter.push_back(counter);
        }

        switch(coolingMethod) {
            case 1:
                temperature *= CalculateTemperatureBoltzmann();
                break;
            case 2:
                temperature *= CalculateTemperatureGeometric();
                break;
        }
        iteration++;
    }

    path = tempVector;
    minCost = minCurrentCost;
}

std::vector<int> SimulatedAnnealing::ShuffleVertices(std::vector<int> vector, int numberOfVertices) {
    std::vector<int> temp = vector;
    int x, y;
    for(int i = 0; i < 100; i++) {
        do {
            x = rand() % numberOfVertices;
            y = rand() % numberOfVertices;
        } while (x == y);
        std::swap(temp.at(x), temp.at(y));
    }
    return temp;
}

std::vector<int> SimulatedAnnealing::swapTwo(std::vector<int> vector, int numberOfVertices){
    std::vector<int> temp = vector;
    int x, y;
    do {
        x = rand() % numberOfVertices;
        y = rand() % numberOfVertices;
    } while (x == y);

    std::swap(temp.at(x), temp.at(y));
    return temp;
}

std::vector<int> SimulatedAnnealing::swapArc(std::vector<int> vector, int numberOfVertices) {
    std::vector<int> temp = vector;
    int x, y;
    do {
        x = rand() % numberOfVertices;
        y = rand() % numberOfVertices;
    } while (x == y);

    if (x > y){
        std::reverse(temp.begin() + y, temp.begin() + x + 1);
    } else {
        std::reverse(temp.begin() + x, temp.begin() + y + 1);
    }
    return temp;
}

int SimulatedAnnealing::CalculateCost(std::vector<int> vector) {
    int cost = 0;

    for(auto i = 0; i < vector.size() - 1; i++){
        cost += vertices[vector.at(i)][vector.at(i + 1)];
    }

    cost += this->vertices[vector.at(vector.size() - 1)][vector.at(0)];
    return cost;
}

float SimulatedAnnealing::CalculateProbability(int newCost, int oldCost, double temperature) {
    float result;

    result = (exp((static_cast<float>((newCost - oldCost) /  ((float)temperature)))));

    return result;
}

double SimulatedAnnealing::CalculateTemperatureBoltzmann() {
    return 1/log(1 + static_cast<double>(this->iteration));
}

double SimulatedAnnealing::CalculateTemperatureGeometric() {
    return pow(coolingCoefficient, iteration);
}

void SimulatedAnnealing::autotests(int numberOfVertices, int ** vertices, int numberOfTries, std::string outputfile) {
    std::ofstream output;
    output.open(outputfile, std::ios::app);
    if (!output){
        std::cout<<"Nie ma pliku wyjsciowego" << std::endl;
    }

    output << "\n\nFILE/NUMBER OF VERTICES:" << numberOfVertices << std::endl;
    std::cout<< "\n\nFILE/NUMBER OF VERTICES:" << numberOfVertices << std::endl;

    minCost = 0;
    path.resize(numberOfVertices);
    iteration = 1;
    this->numberOfVertices = numberOfVertices;
    this->vertices = vertices;

    for(int i = 0; i < numberOfVertices; i++){
        path.at(i) = i;
    }

    path = ShuffleVertices(path, numberOfVertices);

    int startCost = CalculateCost(path);

    int eraLengthsBoltzmann[4] = {1, 100, 10000, 1000000};
    int eraLengthsGeo[4] = {1, 10, 100, 1000};

    double boltzmannTemps[3] = {2, 5, 10};
    double geoTemps[4] = {1.5, 10, 100, 1000};

    double geoCoolings[4] = {0.8, 0.99, 0.9999, 0.999999};

    coolingMethod = 2;
    swapMethod = 2;
    startTemperature = 1000;
    eraLength = 100;
    coolingCoefficient = 0.9999;

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    algorithm(startCost, path);
    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
    output << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
    for(int i = 0; i < this->results.size(); i++){
        output << results.at(i) << ";" << counter.at(i) << std::endl;
    }
    results.clear();
    counter.clear();

    /*for (int j = 1; j < 3; j++){
        coolingMethod = j;
        std::cout<<"Cooling method: "<< j << std::endl;
        output << "Cooling method: " << j << std::endl;
        for (int k = 1; k < 3; k++){
            swapMethod = k;
            std::cout<<"Swap method: "<< k << std::endl;
            output << "Swap method: " << k << std::endl;
            if (coolingMethod == 1){
                for (double temp:boltzmannTemps){
                    startTemperature = temp;
                    std::cout<<"Start temperature: "<< temp << std::endl;
                    output << "Start temperature: " << temp << std::endl;
                    for (int era:eraLengthsBoltzmann) {
                        eraLength = era;
                        std::cout<< "Era length: "<< era << std::endl;
                        output << "Era length: " << era << std::endl;
                        for (int t = 0; t < numberOfTries; t++) {
                            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
                            algorithm(startCost, path);
                            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
                            output << minCost << ";";
                            output << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
                        }
                    }
                }
            }
            else if (coolingMethod == 2){
                for (double temp:geoTemps){
                    startTemperature = temp;
                    std::cout<<"Start temperature: "<< temp << std::endl;
                    output << "Start temperature: " << temp << std::endl;
                    for (double coeff:geoCoolings){
                        coolingCoefficient = coeff;
                        std::cout<<"Cooling coefficient: "<< coeff << std::endl;
                        output << "Cooling coefficient: " << coeff << std::endl;
                        for (int era:eraLengthsGeo) {
                            eraLength = era;
                            std::cout << "Era length: " << era << std::endl;
                            output << "Era length: " << era << std::endl;
                            for (int t = 0; t < numberOfTries; t++) {
                                std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
                                algorithm(startCost, path);
                                std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
                                output << minCost << ";";
                                output << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
*/
    output.close();
}

double SimulatedAnnealing::getTime(double a, double b){
    return (double(a - b) / CLOCKS_PER_SEC);
}

void SimulatedAnnealing::initializeFromIni(int coolingMethod, int swapMethod, double startTemperature,
                                           double coolingCoefficient, double eraLength) {
    this->coolingMethod = coolingMethod;
    this->swapMethod = swapMethod;
    this->startTemperature = startTemperature;
    this->coolingCoefficient = coolingCoefficient;
    this->eraLength = eraLength;
}
