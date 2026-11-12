#include "ObjectLoader.h"
#include <glad/glad.h>

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#include "tiny_obj_loader.h"




ObjectLoader::Mesh ObjectLoader::LoadObject(const std::string& filepath, const std::string& mtlBasePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), mtlBasePath.c_str());

    if (!warn.empty()) {
        std::cout << "TinyObjLoader Warning: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "TinyObjLoader Error: " << err << std::endl;
    }
    if (!ret) {
        throw std::runtime_error("Failed to load OBJ file: " + filepath);
    }

    std::vector<float> vertices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (index.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else {
                float x = attrib.vertices[3 * index.vertex_index + 0];
                float y = attrib.vertices[3 * index.vertex_index + 1];
                float z = attrib.vertices[3 * index.vertex_index + 2];

                float nx = (index.normal_index >= 0) ? std::abs(attrib.normals[3 * index.normal_index + 0]) : 0.0f;
                float ny = (index.normal_index >= 0) ? std::abs(attrib.normals[3 * index.normal_index + 1]) : 1.0f;
                float nz = (index.normal_index >= 0) ? std::abs(attrib.normals[3 * index.normal_index + 2]) : 0.0f;

                float tileScale = 1.0f;
                if (nx >= ny && nx >= nz) {
                    vertices.push_back(z * tileScale);
                    vertices.push_back(y * tileScale);
                }
                else if (ny >= nx && ny >= nz) {
                    vertices.push_back(x * tileScale);
                    vertices.push_back(z * tileScale);
                }
                else {
                    vertices.push_back(x * tileScale);
                    vertices.push_back(y * tileScale);
                }
            }
        }
    }

    ObjectLoader::Mesh mesh;
    mesh.vertexCount = static_cast<GLsizei>(vertices.size() / 8);

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);



    GLsizei stride = 8 * sizeof(float);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    return mesh;
}
