#include "sphere.h"

#include <cmath>
#include <vector>
#include <glm\glm.hpp>

Sphere::Sphere()
{
    init(48);
}

Sphere::Sphere(size_t prec)
{
    init(prec);
}

float Sphere::toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}
void Sphere::init(size_t prec)
{
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    vertices.resize(numVertices);
    texCoords.resize(numVertices);
    normals.resize(numVertices);
    indices.resize(numIndices, 0);

    for (size_t i = 0; i <= prec; i++)
    {
        for (size_t j = 0; j <= prec; j++)
        {
            float y = std::cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -std::cos(toRadians(j * 360.0f / prec)) * std::abs(std::cos(std::asin(y)));
            float z = std::sin(toRadians(j * 360.0f / prec)) * std::abs(std::cos(std::asin(y)));

            vertices.at(i * (prec + 1) + j) = glm::vec3(x, y, z);
            texCoords.at(i * (prec + 1) + j) = glm::vec2(static_cast<float>(j) / prec, static_cast<float>(i) / prec);
            normals.at(i * (prec + 1) + j) = glm::vec3(x, y, z);
        }
    }
    for (size_t i = 0; i < prec; i++)
    {
        for (size_t j = 0; j < prec; j++)
        {
            indices.at(6 * (i * prec + j) + 0) = i * (prec + 1) + j;
            indices.at(6 * (i * prec + j) + 1) = i * (prec + 1) + j + 1;
            indices.at(6 * (i * prec + j) + 2) = (i + 1) * (prec + 1) + j;
            indices.at(6 * (i * prec + j) + 3) = i * (prec + 1) + j + 1;
            indices.at(6 * (i * prec + j) + 4) = (i + 1) * (prec + 1) + j + 1;
            indices.at(6 * (i * prec + j) + 5) = (i + 1) * (prec + 1) + j;
        }
    }
}

size_t Sphere::getNumVertices() const { return numVertices; }
size_t Sphere::getNumIndices() const { return numIndices; }
const std::vector<size_t> &Sphere::getIndices() const { return indices; }
const std::vector<glm::vec3> &Sphere::getVertices() const { return vertices; }
const std::vector<glm::vec2> &Sphere::getTexCoords() const { return texCoords; }
const std::vector<glm::vec3> &Sphere::getNormals() const { return normals; }
