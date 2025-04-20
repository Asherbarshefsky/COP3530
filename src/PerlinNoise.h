#ifndef PERLINNOISE_PERLINNOISE_H
#define PERLINNOISE_PERLINNOISE_H


class PerlinNoise {
private:
    int seed;
    int scale;
    int octaves;

    struct Vector {
        float x;
        float y;
    };

    static float ease(float x);

    static float lerp(float a, float b, float t);

    [[nodiscard]] float getRandomGradient(int x) const;

    static int generateSeed(int x);

public:
    explicit PerlinNoise(int scale, int octaves = -1, int seed = -1);

    float getNoise(int x) const;

    void setSeed(int x);
};


#endif
