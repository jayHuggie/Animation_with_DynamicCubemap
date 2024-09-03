#ifndef SPHERE_H
#define SPHERE_H
#include "core.h"

void generateSphereData(std::vector<float>& vertices, std::vector<unsigned int>& indices, int latitudeCount, int longitudeCount) {
    float PI = 3.14159265358979323846f;

    // Clear any existing data
    vertices.clear();
    indices.clear();

    // Generate positions and normals
    for (int latitude = 0; latitude <= latitudeCount; ++latitude) {
        for (int longitude = 0; longitude <= longitudeCount; ++longitude) {
            float theta = latitude * PI / latitudeCount;
            float phi = longitude * 2 * PI / longitudeCount;
            float x = sinf(theta) * cosf(phi);
            float y = cosf(theta);
            float z = sinf(theta) * sinf(phi);

            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normal (same as position for a sphere centered at the origin)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Texture Coordinates
            float u = (float)longitude / longitudeCount;
            float v = (float)latitude / latitudeCount;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    // Generate indices
    for (int latitude = 0; latitude < latitudeCount; ++latitude) {
        for (int longitude = 0; longitude < longitudeCount; ++longitude) {
            int first = (latitude * (longitudeCount + 1)) + longitude;
            int second = first + longitudeCount + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

#endif // SPHERE_H