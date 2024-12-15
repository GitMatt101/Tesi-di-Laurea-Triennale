#include "../api/shapeBuilder.hpp"

pair<vector<Vertex*>, vector<GLuint>> ShapeBuilder::createBox(float width, float height, float depth, vec4 color) {
    float x = width / 2;
    float y = height / 2;
    float z = depth / 2;

    vector<Vertex* > vertices;

    // Front
    vertices.push_back(new Vertex(vec3(-x, y, z), color));
    vertices.push_back(new Vertex(vec3(x, y, z), color));
    vertices.push_back(new Vertex(vec3(x, -y, z), color));
    vertices.push_back(new Vertex(vec3(-x, -y, z), color));

    // Back
    vertices.push_back(new Vertex(vec3(-x, y, -z), color));
    vertices.push_back(new Vertex(vec3(x, y, -z), color));
    vertices.push_back(new Vertex(vec3(x, -y, -z), color));
    vertices.push_back(new Vertex(vec3(-x, -y, -z), color));

    // Indexes
    vector<GLuint> indices = {
        // Front face
        0, 1, 1, 2, 2, 3, 3, 0,
        // Back face
        4, 5, 5, 6, 6, 7, 7, 4,
        // Connecting edges
        0, 4, 1, 5, 2, 6, 3, 7
    };

    return { vertices, indices };
}

void ShapeBuilder::initShape(Shape* shape) {
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