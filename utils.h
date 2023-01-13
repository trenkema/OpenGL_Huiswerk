#pragma once
int heightMapWidth, heightMapHeight, hScale;

void loadFromFile(const char* url, char** target) {
    std::ifstream stream(url, std::ios::binary);

    stream.seekg(0, stream.end);
    int total = stream.tellg();
    *target = new char[total + 1];

    stream.seekg(0, stream.beg);
    stream.read(*target, total);

    (*target)[total] = '\0';

    stream.close();
}

unsigned int loadTexture(std::string url, GLenum format, int comp ) {
    // gen & bind IDs
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // instellen texture params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // shit inladen
    int width, height, channels;
    unsigned char* data;
    data = stbi_load(url.c_str(), &width, &height, &channels, comp);
    if (data == nullptr) {
        std::cout << "Error loading file: " << url.c_str() << std::endl;
    }
    else {
        // shit aan opengl geven
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void CreateShader(const char* url, GLenum type, unsigned int& shader) {
    static int success;
    static char infoLog[512];

    char* target;
    loadFromFile(url, &target);

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &target, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

//unsigned int GeneratePlane(const char* heightmap, GLenum format, int comp, float hScale, float xzScale, unsigned int& size, unsigned int& heightmapID) {
//    //int width = 0, height = 0, channels;
//    unsigned char* data = nullptr;
//    int channels;
//    //unsigned char* data = nullptr;
//    if (heightmap != nullptr) {
//        data = stbi_load(heightmap, &heightMapWidth, &heightMapHeight, &channels, comp);
//        if (data) {
//            glGenTextures(1, &heightmapID);
//            glBindTexture(GL_TEXTURE_2D, heightmapID);
//
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//            glTexImage2D(GL_TEXTURE_2D, 0, format, heightMapWidth, heightMapHeight, 0, format, GL_UNSIGNED_BYTE, data);
//            glGenerateMipmap(GL_TEXTURE_2D);
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
//        else
//        {
//            printf("DATA IS NULL");
//        }
//    }
//
//    int stride = 8;
//    float* vertices = new float[(heightMapWidth * heightMapHeight) * stride];
//    unsigned int* indices = new unsigned int[(heightMapWidth - 1) * (heightMapHeight - 1) * 6];
//
//    int index = 0;
//
//    //for (int i = 0; sizeof(newModels); i++)
//    //{
//    //    int y = data[((int)newModels[i].position.x * heightMapWidth + (int)newModels[i].position.z) * channels] * hScale;
//    //    newModels[i].position.y = y;
//    //}
//
//    //int y = data[(25 * heightMapWidth + 25) * channels] * hScale;
//    //modelPosition = glm::vec3(25, y, 25);
//
//    //printf("Y: %d", y / 255);
//
//    for (int i = 0; i < (heightMapWidth * heightMapHeight); i++) {
//        // TODO: calculate x/z values
//        int x = i % heightMapWidth;
//        int z = i / heightMapWidth;
//
//        // TODO: set position
//        vertices[index++] = x * xzScale;
//        vertices[index++] = 0;  //dit is waar je de pixel zou lezen...
//        vertices[index++] = z * xzScale;
//
//        // TODO: set normal
//        vertices[index++] = 0;
//        vertices[index++] = 1;
//        vertices[index++] = 0;
//
//        // TODO: set uv
//        vertices[index++] = x / (float)(heightMapWidth - 1);
//        vertices[index++] = z / (float)(heightMapHeight - 1);
//    }
//
//    // OPTIONAL TODO: Calculate normal
//    // TODO: Set normal
//
//    index = 0;
//    for (int i = 0; i < (heightMapWidth - 1) * (heightMapHeight - 1); i++) {
//        int x = i % (heightMapWidth - 1);
//        int z = i / (heightMapWidth - 1);
//
//        int vertex = z * heightMapWidth + x;
//
//        indices[index++] = vertex;
//        indices[index++] = vertex + heightMapWidth + 1;
//        indices[index++] = vertex + 1;
//
//        indices[index++] = vertex;
//        indices[index++] = vertex + heightMapWidth;
//        indices[index++] = vertex + heightMapWidth + 1;
//    }
//
//    unsigned int vertSize = (heightMapWidth * heightMapHeight) * stride * sizeof(float);
//    size = ((heightMapWidth - 1) * (heightMapHeight - 1) * 6) * sizeof(unsigned int);
//
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
//
//    // vertex information!
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, 0);
//    glEnableVertexAttribArray(0);
//    // normal
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 3));
//    glEnableVertexAttribArray(1);
//    // uv
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 6));
//    glEnableVertexAttribArray(2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);
//
//    delete[] vertices;
//    delete[] indices;
//
//    stbi_image_free(data);
//
//    return VAO;
//}