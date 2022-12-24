#ifndef ALGO_CUBE_H
#define ALGO_CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algo/shader.h>

class cube {
    glm::vec3 position_;
    unsigned int VAO_;

public:
    explicit cube(glm::vec3 position);

    void Draw(Shader &lightCubeShader, const glm::mat4& view, const glm::mat4& projection);
};

#endif //ALGO_CUBE_H
