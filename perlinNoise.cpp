// CSCI 411
// Advanced Algorithms Project - Perlin Noise Generation
// Author: Adrian Spinali

#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <cmath>
using namespace std;

// Returns a unit vector (x, y) given an angle
vector<float> getUnitVector(float angle)
{
    return {cos(angle), sin(angle)};
}

// Returns a 2D vector of float vectors where each vector is a direction at a point on the grid
vector<vector<vector<float>>> generateNoiseMap(int width, int height){
    // Make a new random number generator to generate vector angles
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<float> getAngle(0.0, (float)M_PI * 2);
    
    // Create a 2D vector of vectors
    vector<vector<vector<float>>> noiseMap(width, vector<vector<float>>(height, vector<float>(2, 0)));

    // Iterate for each column in each row
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Create a new random unit vector
            noiseMap[i][j] = getUnitVector(getAngle(generator));
        }
    }

    return noiseMap;
}

// Returns the dot product of 2 2D vectors
float dotProduct(vector<float> v1, vector<float> v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1];
}

// Computers linear interpolation between two values
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

// Passes a float t through the fade function 6t^5-15t^4+10t^3
float fadeFunction(float t)
{
    return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

// Returns a single float value between 1 and -1 representing the noise value at a given point
// Assumes that xPosition and yPosition are points on the noiseMap (checked in main)
float applyNoise(vector<vector<vector<float>>>& noiseMap, float xPosition, float yPosition){
    // Get 4 dot products for each corner of the enclosing square
    // Each dot product is composed of the random noise vector at that corner and the distance vector from that corner to the target point
    float topLeft = dotProduct(noiseMap[(int)xPosition][(int)yPosition], {xPosition - (int)xPosition, yPosition - (int)yPosition});
    float topRight = dotProduct(noiseMap[(int)xPosition + 1][(int)yPosition], {xPosition - (int)(xPosition + 1), yPosition - (int)yPosition});
    float bottomLeft = dotProduct(noiseMap[(int)xPosition][(int)yPosition + 1], {xPosition - (int)xPosition, yPosition - (int)(yPosition + 1)});
    float bottomRight = dotProduct(noiseMap[(int)xPosition + 1][(int)yPosition + 1], {xPosition - (int)(xPosition + 1), yPosition - (int)(yPosition + 1)});

    // Use lerp function to linearly interpolate between 2 values
    float topCenter = lerp(topLeft, topRight, fadeFunction(xPosition - (int)xPosition));
    float bottomCenter = lerp(bottomLeft, bottomRight, fadeFunction(xPosition - (int)xPosition));
    float trueCenter = lerp(topCenter, bottomCenter, fadeFunction(yPosition - (int)yPosition));

    return trueCenter;
}

int main(){
    cout << "Enter two positive integers representing the width and height of the noise grid: ";
    int gridWidth = -1;
    int gridHeight = -1;
    cin >> gridWidth >> gridHeight;
    if (gridWidth < 1 || gridHeight < 1)
    {
        cerr << "Error: grid dimensions must be positive values." << endl;
        return -1;
    }
    vector<vector<vector<float>>> noiseMap = generateNoiseMap(gridWidth + 1, gridHeight + 1);
    cout << endl;
    cout << "Enter the target position within the bounds of the grid: ";

    float positionX = -1;
    float positionY = -1;
    while (cin >> positionX >> positionY)
    {
        if (positionX < 0 || positionY < 0 || positionX > gridWidth || positionY > gridHeight)
        {
            cerr << "Error: target position must be within the bounds of the grid." << endl;
        }
        else
        {
            cout << "Noise: " << applyNoise(noiseMap, positionX, positionY) << endl;
        }
        cout << "Enter the target position within the bounds of the grid: ";
    }
  
    return 0;
}