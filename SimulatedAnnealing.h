#include "includes.h"

class SimulatedAnnealing {

private:
    std::vector<int> path;
    int numberOfVertices;
    int minCost;
    int ** vertices;
    int coolingMethod;
    int swapMethod;
    int eraLength;
    std::vector<int> results;
    std::vector<int> counter;
    double startTemperature;
    double coolingCoefficient;
    double iteration;
public:
    void run(int numberOfVertices, int **vertices, std::string outputfile);
    void algorithm(int startCost, std::vector<int> startPath);
    std::vector<int> ShuffleVertices(std::vector<int> vector, int numberOfVertices);
    std::vector<int> swapTwo(std::vector<int> vector, int numberOfVertices);
    std::vector<int> swapArc(std::vector<int> vector, int numberOfVertices);
    int CalculateCost(std::vector<int> vector);
    float CalculateProbability(int newCost, int oldCost, double temperature);
    double CalculateTemperatureGeometric();
    double CalculateTemperatureBoltzmann();
    void autotests(int numberOfVertices, int ** vertices, int numberOfTries, std::string outputfile);
    double getTime(double a, double b);
    void initializeFromIni(int coolingMethod, int swapMethod, double startTemperature,
                           double coolingCoefficient, double eraLength);
};

