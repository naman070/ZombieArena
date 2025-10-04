#pragma once
#include <random>

class RandomGenerator{
    private:
        std::mt19937 gen;

        // private constructor
        RandomGenerator() : gen(std::random_device{}()) {}

        // private destructor
        ~RandomGenerator(){}

        // delete copy constructor and assignment operator
        RandomGenerator(const RandomGenerator&) = delete;
        RandomGenerator& operator = (const RandomGenerator&) = delete;

    public:
        static RandomGenerator& getInstance(){
            static RandomGenerator instance;
            return instance;
        }

        int getInt(int min, int max){
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        }
};