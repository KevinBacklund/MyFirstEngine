#include "ObjectLoader.h"
#include <glad/glad.h>

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#include "tiny_obj_loader.h"


ObjectLoader::Mesh ObjectLoader::MyLoadObject(const std::string& filepath)
{
    Mesh mesh{};
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << filepath << "\n";
        return mesh; 
    }

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec2> tempTexCoords;
    std::vector<glm::vec3> tempNormals;
    std::vector<Vertex> vertices;

    std::string currentObjectName;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "o")
        {
            iss >> currentObjectName;
        }
        else if (prefix == "v")
        {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vt")
        {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            tempTexCoords.push_back(uv);
        }
        else if (prefix == "vn")
        {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            tempNormals.push_back(norm);
        }
        else if (prefix == "f")
        {
            std::vector<Vertex> faceVertices;
            std::string token;

            while (iss >> token)
            {
                faceVertices.push_back(
                    ParseFaceVertex(token, tempPositions, tempTexCoords, tempNormals)
                );
            }

            for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
            {
                if(!faceVertices[0].hasNormal || !faceVertices[i].hasNormal || !faceVertices[i + 1].hasNormal)
                {
                    glm::vec3 edge1 = faceVertices[i].position - faceVertices[0].position;
                    glm::vec3 edge2 = faceVertices[i + 1].position - faceVertices[0].position;
                    glm::vec3 crossProd = glm::cross(edge1, edge2);

                    glm::vec3 faceNormal = glm::normalize(crossProd);
                   
                    if(!faceVertices[0].hasNormal)
                    {
                        faceVertices[0].normal = faceNormal;
                    }
                    if(!faceVertices[i].hasNormal)
                    {
                        faceVertices[i].normal = faceNormal;
                    }
                    if(!faceVertices[i + 1].hasNormal)
                    {
                        faceVertices[i + 1].normal = faceNormal;
                    }
                }
                vertices.push_back(faceVertices[0]);
                vertices.push_back(faceVertices[i]);
                vertices.push_back(faceVertices[i + 1]);
            }
        }
    }

    if (vertices.empty())
    {
        std::cerr << "OBJ file contained no valid face data: " << filepath << "\n";
        return mesh;
    }

    for (size_t i = 0; i < vertices.size(); ++i)
    {

        if (!vertices[i].hasTexCoord)
        {
            glm::vec3 absNorm = glm::abs(vertices[i].normal);

            if (absNorm.x >= absNorm.y && absNorm.x >= absNorm.z) {
                vertices[i].texCoord = glm::vec2(vertices[i].position.z, vertices[i].position.y);
            }
            else if (absNorm.y >= absNorm.x && absNorm.y >= absNorm.z) {
                vertices[i].texCoord = glm::vec2(vertices[i].position.x, vertices[i].position.z);
            }
            else {
                vertices[i].texCoord = glm::vec2(vertices[i].position.x, vertices[i].position.y);
            }
        }
    }

    mesh.vertexCount = vertices.size();

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return mesh;
}

ObjectLoader::Vertex ObjectLoader::ParseFaceVertex(const std::string& faceVertex, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals)
{
    ObjectLoader::Vertex vertex{};
    size_t firstSlash = faceVertex.find('/');

    try {
        int positionIndex = std::stoi(faceVertex.substr(0, firstSlash)) - 1;
        if (positionIndex >= 0 && positionIndex < static_cast<int>(positions.size())) {
            vertex.position = positions[positionIndex];
        }

        if (firstSlash != std::string::npos) {
            size_t secondSlash = faceVertex.find('/', firstSlash + 1);

            std::string vtStr = (secondSlash == std::string::npos)
                ? faceVertex.substr(firstSlash + 1)
                : faceVertex.substr(firstSlash + 1, secondSlash - firstSlash - 1);

            if (!vtStr.empty()) {
                int texCoordIndex = std::stoi(vtStr) - 1;
                if (texCoordIndex >= 0 && texCoordIndex < static_cast<int>(texCoords.size())) {
                    vertex.texCoord = texCoords[texCoordIndex];
					vertex.hasTexCoord = true;
                }
            }

            if (secondSlash != std::string::npos && secondSlash + 1 < faceVertex.size()) {
                int normalIndex = std::stoi(faceVertex.substr(secondSlash + 1)) - 1;
                if (normalIndex >= 0 && normalIndex < static_cast<int>(normals.size())) {
                    vertex.normal = normals[normalIndex];
					vertex.hasNormal = true;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing face vertex: " << faceVertex << ". Exception: " << e.what() << std::endl;
    }

    return vertex;
}

ObjectLoader::Mesh ObjectLoader::LoadObjectTiny(const std::string& filepath, const std::string& mtlBasePath)
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
