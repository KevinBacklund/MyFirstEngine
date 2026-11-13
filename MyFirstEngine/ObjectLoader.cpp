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
                size_t currIdx = &index - shape.mesh.indices.data();
                size_t triStart = (currIdx / 3) * 3;

                int i0 = shape.mesh.indices[triStart + 0].vertex_index;
                int i1 = shape.mesh.indices[triStart + 1].vertex_index;
                int i2 = shape.mesh.indices[triStart + 2].vertex_index;

                float edge1x = attrib.vertices[3 * i1 + 0] - attrib.vertices[3 * i0 + 0];
                float edge1y = attrib.vertices[3 * i1 + 1] - attrib.vertices[3 * i0 + 1];
                float edge1z = attrib.vertices[3 * i1 + 2] - attrib.vertices[3 * i0 + 2];

                float edge2x = attrib.vertices[3 * i2 + 0] - attrib.vertices[3 * i0 + 0];
                float edge2y = attrib.vertices[3 * i2 + 1] - attrib.vertices[3 * i0 + 1];
                float edge2z = attrib.vertices[3 * i2 + 2] - attrib.vertices[3 * i0 + 2];

                float fnx = edge1y * edge2z - edge1z * edge2y;
                float fny = edge1z * edge2x - edge1x * edge2z;
                float fnz = edge1x * edge2y - edge1y * edge2x;

                float len = std::sqrt(fnx * fnx + fny * fny + fnz * fnz);
                if (len > 0.0f) {
                    fnx /= len;
                    fny /= len;
                    fnz /= len;
                }

                vertices.push_back(fnx);
                vertices.push_back(fny);
                vertices.push_back(fnz);
            }

            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else 
            {
                float vx = attrib.vertices[3 * index.vertex_index + 0];
                float vy = attrib.vertices[3 * index.vertex_index + 1];
                float vz = attrib.vertices[3 * index.vertex_index + 2];

                float nx = (index.normal_index >= 0) ? attrib.normals[3 * index.normal_index + 0] : 0.0f;
                float ny = (index.normal_index >= 0) ? attrib.normals[3 * index.normal_index + 1] : 0.0f;
                float nz = (index.normal_index >= 0) ? attrib.normals[3 * index.normal_index + 2] : 1.0f;

                float absX = std::abs(nx);
                float absY = std::abs(ny);
                float absZ = std::abs(nz);

                if (absX >= absY && absX >= absZ) {
                    vertices.push_back(vz);
                    vertices.push_back(vy);
                }
                else if (absY >= absX && absY >= absZ) {
                    vertices.push_back(vx);
                    vertices.push_back(vz);
                }
                else {
                    vertices.push_back(vx);
                    vertices.push_back(vy);
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
