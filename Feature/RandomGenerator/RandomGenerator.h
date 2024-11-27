#pragma once

#include <random>
#include <Vector3.h>


class RandomGenerator
{
public:
    RandomGenerator();
    ~RandomGenerator() = default;
    static int GetRandom(int min, int max);
    static float GetRandom(float min, float max);
    static double GetRandom(double min, double max);
    static Vector3 GetRandom(Vector3 min, Vector3 max);

    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

private:
    static std::mt19937 engine;
};