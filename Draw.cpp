#include <glad/glad.h>

void DrawParticles(GLuint vao, GLsizei count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, count);
}