#include <iostream>
#include <random>
#include <ctime>
#include <cmath>

#include "PerlinNoise.h"

float PerlinNoise::ease(const float x) {
    return static_cast<float>(6 * std::pow(x, 5) - 15 * std::pow(x, 4) + 10 * std::pow(x, 3));
}

float PerlinNoise::lerp(const float a, const float b, const float t) {
    return a + (b - a) * t;
}

float PerlinNoise::getRandomGradient(int x) const {
    // Middle square hash function
    x = x * seed + seed;
    x = static_cast<int>(std::pow(x % static_cast<int>(1e4), 2));
    x = x % static_cast<int>(1e6) / 100;
    const auto random = static_cast<float>(sin(x));
    return random;
}

int PerlinNoise::generateSeed(const int x) {
    // Generate random seed based on input value, use current time as default value
    srand(std::time(nullptr));

    // -1 indicates default value
    if (x > -1)
        srand(x);

    const int seed = rand() % static_cast<int>(1e8);

    return seed;
}


PerlinNoise::PerlinNoise(const int scale, const int octaves, const int seed) {
    this->scale = scale;

    // -1 indicates default octave value
    if (octaves == -1)
        this->octaves = std::min(static_cast<int>(std::log2(scale) + 1), 8);
    else
        this->octaves = octaves;

    this->seed = generateSeed(seed);
}

float PerlinNoise::getNoise(const int x) const {
    float result = 0.f;

    // Get noise for each octave
    for (int i = 0; i < octaves; i++) {
        const int octScale = scale / static_cast<int>(std::pow(2, i));

        const int left = octScale * (x / octScale);
        const int right = left + octScale;

        const float g1 = getRandomGradient(left);
        const float g2 = getRandomGradient(right);

        const float d1 = static_cast<float>(x - left) / static_cast<float>(octScale);
        const float d2 = static_cast<float>(x - right) / static_cast<float>(octScale);

        const float v1 = g1 * d1;
        const float v2 = g2 * d2;

        const float f = ease(d1);
        const float val = lerp(v1, v2, f);

        result += static_cast<float>(val / std::pow(2, i));
    }

    return static_cast<float>(result * static_cast<float>(std::pow(2, octaves)) / (std::pow(2, octaves) - 1));
}

void PerlinNoise::setSeed(const int x) {
    this->seed = generateSeed(x);
}
