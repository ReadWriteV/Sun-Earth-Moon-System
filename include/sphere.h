#pragma once

#include <vector>
#include <glm\glm.hpp>

class Sphere
{
public:
    Sphere();
    Sphere(size_t prec);
    size_t getNumVertices() const;
    size_t getNumIndices() const;
    const std::vector<size_t> &getIndices() const;
    const std::vector<glm::vec3> &getVertices() const;
    const std::vector<glm::vec2> &getTexCoords() const;
    const std::vector<glm::vec3> &getNormals() const;

    static float toRadians(float degrees);

private:
    void init(size_t prec);

private:
    size_t numVertices;
    size_t numIndices;
    std::vector<size_t> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
};