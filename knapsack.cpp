#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

void checkInput(std::ifstream &inFS,int argc, char* argv[], int input) {
    if (!inFS.is_open()) {
        throw std::runtime_error( "Could not open file ");
    }
}

void divideInput(std::vector<int> &inputVec, std::vector<int> &weight, std::vector<int> &value ) {
    for (int i = 1; i < inputVec.size()-1; i++) {
        if (i % 2 != 0) {
            weight.push_back(inputVec.at(i));
        }
        else {
            value.push_back(inputVec.at(i));
        }
    }
}

int total(std::vector<int> &weight, std::vector<bool> &included, int totalWeight) {
    for (int j = 0; j < weight.size(); j++) {
        if (included.at(j)) {
            totalWeight = totalWeight + weight.at(j);
        }
    }
    return totalWeight;
}

int knapsackRecursive( std::vector<int> &weight, std::vector<int> &value, std::vector<bool> &included, int capacityLeft, int index) {
    if (index == weight.size() || capacityLeft == 0) {
        return 0;
    }
    if (weight.at(index) > capacityLeft) {
        return knapsackRecursive(weight, value, included, capacityLeft, index+1);
    }

    std::vector<bool> include = included;
    std::vector<bool> exclude = included;
    int solutionWithout = knapsackRecursive(weight, value, exclude, capacityLeft, index+1);
    include.at(index) = true;
    int solutionWith = value.at(index) + knapsackRecursive( weight, value, include, capacityLeft - weight.at(index), index+1);
    
    if (solutionWith > solutionWithout) {
        included = include;
        return solutionWith;
    }
    else {
        included = exclude;
        return solutionWithout;
    }
}

int main(int argc, char* argv[]) {  
    std::ifstream inFS;   
    int input;
    std::vector<int> inputVec;
    int index = 0;
    std::vector<int> weight;
    std::vector<int> value;
    int totalWeight = 0;

    
    try {
        inFS.open(argv[1]);
        if (argc != 2) {
            throw std::runtime_error( "No input filename given");
            return 1;
        }
        checkInput(inFS,argc,argv,input);
        while (!inFS.fail()) {
            inFS >> input;
            inputVec.push_back(input);
        }
    } catch(std::runtime_error &except) {
        std::cout << std::endl << except.what() << argv[1] << std::endl;
        return 1;
    } 

    int capacity = inputVec.at(0);
    divideInput(inputVec, weight, value);
    std::vector<bool> included(value.size(), false);
    int totalValue = knapsackRecursive(weight, value, included, capacity, 0);
    totalWeight = total(weight, included, totalWeight);

    std::cout << "With a capacity of " << capacity << " kg, the total value is " << totalValue << " Eur, leading to a total weight of " << totalWeight << " kg" << std::endl << "The following items are included: ";
    for (int k = 0; k < weight.size(); k++) {
        if (included.at(k)) {
            std::cout << "[" << weight.at(k)  << "," << value.at(k) << "] ";
        }
    }

    inFS.close(); 

    return 0;
}
 
