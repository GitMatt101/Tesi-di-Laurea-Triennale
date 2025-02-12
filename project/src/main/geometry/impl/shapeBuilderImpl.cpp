#include "../api/shapeBuilder.hpp"

pair<vector<Vertex*>, vector<GLuint>> ShapeBuilder::createBox(float width, float height, float depth, vec4 color) {
    float x = width;
    float y = height;
    float z = depth;

    vector<Vertex* > vertices;

    // Front
    vertices.push_back(new Vertex(vec3(0, y, z), color));
    vertices.push_back(new Vertex(vec3(x, y, z), color));
    vertices.push_back(new Vertex(vec3(x, 0, z), color));
    vertices.push_back(new Vertex(vec3(0, 0, z), color));

    // Back
    vertices.push_back(new Vertex(vec3(0, y, 0), color));
    vertices.push_back(new Vertex(vec3(x, y, 0), color));
    vertices.push_back(new Vertex(vec3(x, 0, 0), color));
    vertices.push_back(new Vertex(vec3(0, 0, 0), color));

    // Indexes
    vector<GLuint> indices = {
        // Front Face
        0, 1, 2, 2, 3, 0,
        // Right Face
        1, 5, 6, 6, 2, 1,
        // Back Face
        7, 6, 5, 5, 4, 7,
        // Left Face
        4, 0, 3, 3, 7, 4,
        // Top Face
        4, 5, 1, 1, 0, 4,
        // Bottom Face
        3, 2, 6, 6, 7, 3
    };
    return { vertices, indices };
}

void ShapeBuilder::initShape(Box* shape) {
	// Initialize VAO
    glGenVertexArrays(1, shape->getVAO());
    glBindVertexArray(*shape->getVAO());

    // Initialize vertices' VBO
    glGenBuffers(1, shape->getVerticesVBO());
    glBindBuffer(GL_ARRAY_BUFFER, *shape->getVerticesVBO());
    glBufferData(
        GL_ARRAY_BUFFER,
        shape->getVerticesCoordinates().size() * sizeof(vec3),
        shape->getVerticesCoordinates().data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Initialize vertices' colors' VBO
    glGenBuffers(1, shape->getColorsVBO());
    glBindBuffer(GL_ARRAY_BUFFER, *shape->getColorsVBO());
    glBufferData(
        GL_ARRAY_BUFFER,
        shape->getVerticesColors().size() * sizeof(vec4),
        shape->getVerticesColors().data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Initialize the shape's EBO
    glGenBuffers(1, shape->getEBO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *shape->getEBO());
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        shape->getIndices().size() * sizeof(GLuint),
        shape->getIndices().data(),
        GL_STATIC_DRAW
    );
}