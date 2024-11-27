#include "RandomGenerator.h"

std::mt19937 RandomGenerator::engine;

RandomGenerator::RandomGenerator()
{
    std::random_device seed_gen;
    engine = std::mt19937(seed_gen());
}

int RandomGenerator::GetRandom(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

float RandomGenerator::GetRandom(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine);
}

double RandomGenerator::GetRandom(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine);
}

Vector3 RandomGenerator::GetRandom(Vector3 min, Vector3 max)
{
    return Vector3(GetRandom(min.x, max.x), GetRandom(min.y, max.y), GetRandom(min.z, max.z));
}
