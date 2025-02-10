#include "mesh.hpp"

// clang-format off
#include <glad/gl.h>
#include <GL/gl.h>
// clang-format on

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

namespace NGameEngine {

static const glm::vec4 kDefaultVertexColor = {1.0f, 0.5f, 0.2f, 1.0f};

// clang-format off
static const TMeshData kPlatformVertexData[] = {
    {{-9.f, -0.5f,  9.f}, kDefaultVertexColor},
    {{-9.f,  0.5f,  9.f}, kDefaultVertexColor},
    {{ 9.f, -0.5f,  9.f}, kDefaultVertexColor},
    {{ 9.f,  0.5f,  9.f}, kDefaultVertexColor},
    {{ 9.f, -0.5f, -9.f}, kDefaultVertexColor},
    {{ 9.f,  0.5f, -9.f}, kDefaultVertexColor},
    {{-9.f, -0.5f, -9.f}, kDefaultVertexColor},
    {{-9.f,  0.5f, -9.f}, kDefaultVertexColor},
};
// clang-format on

static const struct {
    std::array<GLuint, 3> indices;
} kPlatformVertices[] = {
    // front face
    {0, 1, 2},
    {1, 2, 3},
    // right face
    {2, 3, 4},
    {3, 4, 5},
    // back face
    {4, 5, 6},
    {5, 6, 7},
    // left face
    {6, 7, 0},
    {7, 0, 1},
    // top face
    {1, 3, 5},
    {5, 7, 1},
    // bottom face
    {0, 2, 4},
    {4, 6, 0},
};

namespace {

class TMesh : public IMesh {
  public:
    TMesh(GLuint vao, GLuint shader_program, size_t vertices_count);
    ~TMesh() override = default;
    void draw(const glm::mat4x4& mvp) override;

  private:
    GLuint vao_;
    GLuint shader_program_;

    size_t vertices_count_;
};

TMesh::TMesh(GLuint vao, GLuint shader_program, size_t vertices_count)
    : vao_(vao)
    , shader_program_(shader_program)
    , vertices_count_(vertices_count) {
    std::cerr << "Mesh created" << std::endl
              << "vao: " << vao_ << std::endl
              << "shader_program: " << shader_program_ << std::endl
              << "vertices_count: " << vertices_count_ << std::endl;
}

void TMesh::draw(const glm::mat4x4& mvp) {
    auto mvp_location = glGetUniformLocation(shader_program_, "mvp");
    glUseProgram(shader_program_);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, vertices_count_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}  // namespace

static GLuint CreateShader(GLenum shader_type, const char* shader_program) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_program, NULL);
    glCompileShader(shader);

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        std::cerr << "Shader compile failed: " << info_log << std::endl;
        std::exit(3);
    }

    return shader;
}

static const char* kVertexShaderProgram =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec4 color;\n"
    "uniform mat4 mvp;"
    "out vec4 fColor;\n"
    "void main() {\n"
    "    fColor = color;\n"
    "    gl_Position = mvp * vec4(position, 1);\n"
    "}\n";
static inline GLuint CreateVertexShader() {
    return CreateShader(GL_VERTEX_SHADER, kVertexShaderProgram);
}

static const char* kFragmentShaderProgram =
    "#version 330 core\n"
    "in vec4 fColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = fColor;\n"
    "}\n";
static inline GLuint CreateFragmentShader() {
    return CreateShader(GL_FRAGMENT_SHADER, kFragmentShaderProgram);
}

GLuint CreateShaderProgram() {
    GLuint shader_program = glCreateProgram();
    GLuint vertex_shader = CreateVertexShader();
    GLuint fragment_shader = CreateFragmentShader();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        std::cerr << "Shader program link failed: " << info_log << std::endl;
        std::exit(4);
    }
    return shader_program;
}

std::unique_ptr<IMesh> CreatePlatformMesh() {
    GLuint shader_program = CreateShaderProgram();

    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(kPlatformVertexData),
        static_cast<const void*>(kPlatformVertexData), GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(kPlatformVertices),
        static_cast<const void*>(kPlatformVertices), GL_STATIC_DRAW
    );

    GLint vertex_location = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(
        vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(TMeshData),
        reinterpret_cast<void*>(offsetof(TMeshData, position))
    );
    glEnableVertexAttribArray(vertex_location);
    GLint colorLocation = glGetAttribLocation(shader_program, "color");
    glVertexAttribPointer(
        colorLocation, 4, GL_FLOAT, GL_TRUE, sizeof(TMeshData),
        reinterpret_cast<void*>(offsetof(TMeshData, color))
    );
    glEnableVertexAttribArray(colorLocation);

    glBindVertexArray(0);

    return std::make_unique<TMesh>(
        vao, shader_program,
        sizeof(kPlatformVertices) / sizeof(*kPlatformVertices) * 3
    );
}

static std::pair<std::vector<TMeshData>, std::vector<std::array<GLuint, 3>>>
GenerateBallMeshData(float radius) {
    std::vector<TMeshData> mesh_data;
    std::vector<std ::array<GLuint, 3>> vertices;

    static constexpr glm::vec3 kBaseSphereColor = {0.2f, 0.5f, 1.0f};
    static constexpr int kAngleStep = 15;
    static constexpr int kRowCount = 180 / kAngleStep - 1;
    static constexpr int kPointsPerRow = 360 / kAngleStep;

    mesh_data.resize(kPointsPerRow * kRowCount + 2);
    mesh_data[0] = TMeshData{
        .position = glm::vec3{0.f, radius, 0.f},
        .color = glm::vec4(kBaseSphereColor, 1.f),
    };

    for (int i = 0; i < kRowCount; ++i) {
        int theta = (i + 1) * kAngleStep;
        float coordY = glm::cos(glm::radians(static_cast<float>(theta)));
        int alpha = 0;
        for (int j = 1; j <= kPointsPerRow; ++j) {
            float coordX = glm::sin(glm::radians(static_cast<float>(theta))) *
                           glm::sin(glm::radians(static_cast<float>(alpha)));
            float coordZ = glm::sin(glm::radians(static_cast<float>(theta))) *
                           glm::cos(glm::radians(static_cast<float>(alpha)));

            auto color = kBaseSphereColor;
            color[(i + j) % 3] = color[i * j % 3];
            mesh_data[i * kPointsPerRow + j] = TMeshData{
                .position = radius * glm::vec3(coordX, coordY, coordZ),
                .color = glm::vec4(color, 1.f),
            };

            alpha += kAngleStep;
        }
    }
    mesh_data.back() = TMeshData{
        .position = glm::vec3{0.f, -radius, 0.f},
        .color = glm::vec4(kBaseSphereColor, 1.f),
    };

    vertices.resize(kPointsPerRow * 2 * kRowCount);

    for (int i = 0; i < kPointsPerRow - 1; ++i) {
        vertices[i] = {
            0, static_cast<GLuint>(i + 1), static_cast<GLuint>(i + 2)
        };
    }
    vertices[kPointsPerRow - 1] = {0, kPointsPerRow, 1};

    for (int i = 0; i < kPointsPerRow - 1; ++i) {
        vertices[kPointsPerRow + i] = {
            static_cast<GLuint>(mesh_data.size() - 1),
            static_cast<GLuint>((kRowCount - 1) * kPointsPerRow + i + 1),
            static_cast<GLuint>((kRowCount - 1) * kPointsPerRow + i + 2)
        };
    }
    vertices[2 * kPointsPerRow - 1] = {
        static_cast<GLuint>(mesh_data.size() - 1), kRowCount * kPointsPerRow,
        (kRowCount - 1) * kPointsPerRow + 1
    };

    for (int i = 0; i < kRowCount - 1; ++i) {
        int offset = (i + 1) * kPointsPerRow * 2;
        int j;
        for (j = 0; j < kPointsPerRow - 1; ++j) {
            vertices[offset + j * 2] = {
                static_cast<GLuint>((i + 1) * kPointsPerRow + j + 1),
                static_cast<GLuint>(i * kPointsPerRow + j + 1),
                static_cast<GLuint>((i + 1) * kPointsPerRow + j + 2)
            };
            vertices[offset + j * 2 + 1] = {
                static_cast<GLuint>(i * kPointsPerRow + j + 1),
                static_cast<GLuint>((i + 1) * kPointsPerRow + j + 2),
                static_cast<GLuint>(i * kPointsPerRow + j + 2)
            };
        }
        vertices[offset + j * 2] = {
            static_cast<GLuint>((i + 1) * kPointsPerRow + j + 1),
            static_cast<GLuint>(i * kPointsPerRow + j + 1),
            static_cast<GLuint>((i + 1) * kPointsPerRow + 1)
        };
        vertices[offset + j * 2 + 1] = {
            static_cast<GLuint>(i * kPointsPerRow + j + 1),
            static_cast<GLuint>((i + 1) * kPointsPerRow + 1),
            static_cast<GLuint>(i * kPointsPerRow + 1)
        };
    }

    return std::make_pair(mesh_data, vertices);
}

std::unique_ptr<IMesh> CreateBallMesh() {
    GLuint shader_program = CreateShaderProgram();

    const auto& [sphereVertexData, sphereVertexIndices] =
        GenerateBallMeshData(1.f);

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sphereVertexData.size() * sizeof(*sphereVertexData.data()),
        sphereVertexData.data(), GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sphereVertexIndices.size() * sizeof(*sphereVertexIndices.data()),
        sphereVertexIndices.data(), GL_STATIC_DRAW
    );

    GLint vertex_location = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(
        vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(TMeshData),
        reinterpret_cast<void*>(offsetof(TMeshData, position))
    );
    glEnableVertexAttribArray(vertex_location);
    GLint colorLocation = glGetAttribLocation(shader_program, "color");
    glVertexAttribPointer(
        colorLocation, 4, GL_FLOAT, GL_TRUE, sizeof(TMeshData),
        reinterpret_cast<void*>(offsetof(TMeshData, color))
    );
    glEnableVertexAttribArray(colorLocation);

    glBindVertexArray(0);

    return std::make_unique<TMesh>(
        vao, shader_program, sphereVertexIndices.size() * 3
    );
}

}  // namespace NGameEngine
