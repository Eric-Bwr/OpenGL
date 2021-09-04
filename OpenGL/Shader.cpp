#include "Shader.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>

char *Shader::readFile(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == nullptr) {
        errors.failedToLocate = true;
    }
    fseek(file, 0, SEEK_END);
    uint64_t len = ftell(file);
    rewind(file);
    if (len == 0) {
        errors.fileIsEmpty = true;
        fclose(file);
        return nullptr;
    }
    char *data = (char *) malloc(len * sizeof(char) + 1);
    if (data == nullptr) {
        errors.failedToAllocate = true;
        fclose(file);
        return nullptr;
    }
    uint64_t bytes = fread(data, sizeof(char), len, file);
    data[bytes] = '\0';
    fclose(file);
    return data;
}

Shader::Shader(const char *path) : path(path) {
    char *fileData = readFile(path);
    if (!fileData) {
        errors.failedToLocate = true;
    } else {
        int type = -1;
        auto vertex = new std::string;
        auto fragment = new std::string;
        auto geometry = new std::string;
        auto tessControl = new std::string;
        auto tessEval = new std::string;
        auto compute = new std::string;
        std::vector<std::string> splitData;
        std::string _data(fileData);
        std::string buffer;
        for (uint64_t i = 0; i < _data.size(); i++) {
            char letter = _data.at(i);
            if (letter == '\n' || letter == '\r') {
                splitData.emplace_back(buffer);
                buffer.clear();
            } else buffer += letter;
        }
        splitData.emplace_back(buffer);
        for (uint64_t i = 0; i < splitData.size(); i++) {
            char *line = const_cast<char *>(splitData.at(i).data());
            if (strstr(line, "#vertex") != nullptr) {
                type = 0;
            } else if (strstr(line, "#fragment") != nullptr) {
                type = 1;
            } else if (strstr(line, "#geometry") != nullptr) {
                type = 2;
            } else if (strstr(line, "#tessControl") != nullptr) {
                type = 3;
            } else if (strstr(line, "#tessEval") != nullptr) {
                type = 4;
            } else if (strstr(line, "#compute") != nullptr) {
                type = 5;
            } else {
                switch (type) {
                    case -1: {
                        errors.unknownType = true;
                    }
                    case 0: {
                        vertex->append(line);
                        *vertex += "\n";
                        break;
                    }
                    case 1: {
                        fragment->append(line);
                        *fragment += "\n";
                        break;
                    }
                    case 2: {
                        geometry->append(line);
                        *geometry += "\n";
                        break;
                    }
                    case 3: {
                        tessControl->append(line);
                        *tessControl += "\n";
                        break;
                    }
                    case 4: {
                        tessEval->append(line);
                        *tessEval += "\n";
                        break;
                    }
                    case 5: {
                        compute->append(line);
                        *compute += "\n";
                        break;
                    }
                }
            }
        }
        programID = glCreateProgram();
        if (!vertex->empty()) {
            vertID = compileShader(GL_VERTEX_SHADER, vertex->data());
            glAttachShader(programID, vertID);
        }
        if (!fragment->empty()) {
            fragID = compileShader(GL_FRAGMENT_SHADER, fragment->data());
            glAttachShader(programID, fragID);
        }
        if (!geometry->empty()) {
            geoID = compileShader(GL_GEOMETRY_SHADER, geometry->data());
            glAttachShader(programID, geoID);
        }
        if (!tessControl->empty()) {
            tessCID = compileShader(GL_TESS_CONTROL_SHADER, tessControl->data());
            glAttachShader(programID, tessCID);
        }
        if (!tessEval->empty()) {
            tessEID = compileShader(GL_TESS_EVALUATION_SHADER, tessEval->data());
            glAttachShader(programID, tessEID);
        }
        if (!compute->empty()) {
            computeID = compileShader(GL_COMPUTE_SHADER, compute->data());
            glAttachShader(programID, computeID);
        }
        glLinkProgram(programID);
        glValidateProgram(programID);
        delete vertex;
        delete fragment;
        delete geometry;
        delete tessControl;
        delete tessEval;
        delete compute;
    }
    glUseProgram(programID);
}

Shader::Shader() {
    programID = glCreateProgram();
}

void Shader::addFromFile(const char *path) {
    this->path = path;
    char *fileData = readFile(path);
    if (!fileData) {
        errors.failedToLocate = true;
    } else {
        int type = -1;
        auto vertex = new std::string();
        auto fragment = new std::string();
        auto geometry = new std::string();
        auto tessControl = new std::string();
        auto tessEval = new std::string();
        auto compute = new std::string();
        std::vector<std::string> splitData;
        std::string _data(fileData);
        std::string buffer;
        for (char letter : _data) {
            if (letter == '\n' || letter == '\r') {
                splitData.emplace_back(buffer);
                buffer.clear();
            } else buffer += letter;
        }
        splitData.emplace_back(buffer);
        for (auto &i : splitData) {
            char *line = const_cast<char *>(i.data());
            if (strstr(line, "#vertex") != nullptr) {
                type = 0;
            } else if (strstr(line, "#fragment") != nullptr) {
                type = 1;
            } else if (strstr(line, "#geometry") != nullptr) {
                type = 2;
            } else if (strstr(line, "#tessControl") != nullptr) {
                type = 3;
            } else if (strstr(line, "#tessEval") != nullptr) {
                type = 4;
            } else if (strstr(line, "#compute") != nullptr) {
                type = 5;
            } else {
                switch (type) {
                    case -1: {
                        errors.unknownType = true;
                    }
                    case 0: {
                        vertex->append(line);
                        *vertex += "\n";
                        break;
                    }
                    case 1: {
                        fragment->append(line);
                        *fragment += "\n";
                        break;
                    }
                    case 2: {
                        geometry->append(line);
                        *geometry += "\n";
                        break;
                    }
                    case 3: {
                        tessControl->append(line);
                        *tessControl += "\n";
                        break;
                    }
                    case 4: {
                        tessEval->append(line);
                        *tessEval += "\n";
                        break;
                    }
                    case 5: {
                        compute->append(line);
                        *compute += "\n";
                        break;
                    }
                }
            }
        }
        if (!vertex->empty()) {
            vertID = compileShader(GL_VERTEX_SHADER, vertex->data());
            glAttachShader(programID, vertID);
        }
        if (!fragment->empty()) {
            fragID = compileShader(GL_FRAGMENT_SHADER, fragment->data());
            glAttachShader(programID, fragID);
        }
        if (!geometry->empty()) {
            geoID = compileShader(GL_GEOMETRY_SHADER, geometry->data());
            glAttachShader(programID, geoID);
        }
        if (!tessControl->empty()) {
            tessCID = compileShader(GL_TESS_CONTROL_SHADER, tessControl->data());
            glAttachShader(programID, tessCID);
        }
        if (!tessEval->empty()) {
            tessEID = compileShader(GL_TESS_EVALUATION_SHADER, tessEval->data());
            glAttachShader(programID, tessEID);
        }
        if (!compute->empty()) {
            computeID = compileShader(GL_COMPUTE_SHADER, compute->data());
            glAttachShader(programID, computeID);
        }
        delete vertex;
        delete fragment;
        delete geometry;
        delete tessControl;
        delete tessEval;
        delete compute;
    }
}

void Shader::addVertexShader(const char *vertex) {
    vertID = compileShader(GL_VERTEX_SHADER, vertex);
    glAttachShader(programID, vertID);
}

void Shader::addFragmentShader(const char *fragment) {
    fragID = compileShader(GL_FRAGMENT_SHADER, fragment);
    glAttachShader(programID, fragID);
}

void Shader::addGeometryShader(const char *geometry) {
    geoID = compileShader(GL_GEOMETRY_SHADER, geometry);
    glAttachShader(programID, geoID);
}

void Shader::addComputeShader(const char *compute) {
    computeID = compileShader(GL_COMPUTE_SHADER, compute);
    glAttachShader(programID, computeID);
}

void Shader::addTessellationControlShader(const char *tessControl) {
    tessCID = compileShader(GL_TESS_CONTROL_SHADER, tessControl);
    glAttachShader(programID, tessCID);
}

void Shader::addTessellationEvaluationShader(const char *tessEval) {
    tessEID = compileShader(GL_TESS_EVALUATION_SHADER, tessEval);
    glAttachShader(programID, tessEID);
}

void Shader::finish() const {
    if (!hasError()) {
        glLinkProgram(programID);
        glValidateProgram(programID);
        glUseProgram(programID);
    }
}

unsigned int Shader::compileShader(unsigned int type, const char *source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        if (type == GL_VERTEX_SHADER) {
            errors.failedToCompileVertex = true;
            errors.vertexMessage = message;
        } else if (type == GL_FRAGMENT_SHADER) {
            errors.failedToCompileFragment = true;
            errors.fragmentMessage = message;
        } else if (type == GL_GEOMETRY_SHADER) {
            errors.failedToCompileGeometry = true;
            errors.geometryMessage = message;
        } else if (type == GL_TESS_CONTROL_SHADER) {
            errors.failedToCompileTessControl = true;
            errors.tessControlMessage = message;
        } else if (type == GL_TESS_EVALUATION_SHADER) {
            errors.failedToCompileTessEval = true;
            errors.tessEvalMessage = message;
        } else if (type == GL_COMPUTE_SHADER) {
            errors.failedToCompileCompute = true;
            errors.computeMessage = message;
        }
        glDeleteShader(id);
        return 0;
    }
    return id;
}

bool Shader::hasError() const {
    return errors.failedToLocate ||
           errors.failedToAllocate ||
           errors.unknownType ||
           errors.fileIsEmpty ||
           errors.failedToCompileVertex ||
           errors.failedToCompileFragment ||
           errors.failedToCompileGeometry ||
           errors.failedToCompileTessEval ||
           errors.failedToCompileCompute ||
           errors.failedToCompileTessControl;
}

std::string Shader::getErrorMessage() {
    std::string result;
    if (!(path && !path[0])) {
        result.append(path);
        result.append("\n");
    }
    if (!hasError())
        result.append("Successfully loaded");
    if (errors.failedToLocate)
        result.append("Failed to locate Shader\n");
    if (errors.failedToAllocate)
        result.append("Failed to allocate Memory\n");
    if (errors.fileIsEmpty)
        result.append("Faield to load Shader Data -> Empty\n");
    if (errors.unknownType)
        result.append("Failed to get Type\n");
    if (errors.failedToCompileVertex) {
        result.append("\nVertex:\n");
        result.append(errors.vertexMessage);
    }
    if (errors.failedToCompileFragment) {
        result.append("\nFragment:\n");
        result.append(errors.fragmentMessage);
    }
    if (errors.failedToCompileGeometry) {
        result.append("\nGeometry:\n");
        result.append(errors.geometryMessage);
    }
    if (errors.failedToCompileTessEval) {
        result.append("\nTessellation Evaluation:\n");
        result.append(errors.tessEvalMessage);
    }
    if (errors.failedToCompileTessControl) {
        result.append("\nTessellation Control:\n");
        result.append(errors.tessControlMessage);
    }
    if (errors.failedToCompileCompute) {
        result.append("\nCompute:\n");
        result.append(errors.computeMessage);
    }
    result.append("\n");
    return result;
}

void Shader::bind() const {
    glUseProgram(programID);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::reload() {
    errors.failedToLocate = false;
    errors.failedToAllocate = false;
    errors.fileIsEmpty = false;
    errors.unknownType = false;
    errors.failedToCompileVertex = false;
    errors.vertexMessage = "";
    errors.failedToCompileFragment = false;
    errors.fragmentMessage = "";
    errors.failedToCompileGeometry = false;
    errors.geometryMessage = "";
    errors.failedToCompileTessEval = false;
    errors.tessEvalMessage = "";
    errors.failedToCompileTessControl = false;
    errors.tessControlMessage = "";
    errors.failedToCompileCompute = false;
    char *fileData = readFile(path);
    if (!fileData) {
        errors.failedToLocate = true;
    } else {
        int type = -1;
        auto vertex = new std::string;
        auto fragment = new std::string;
        auto geometry = new std::string;
        auto tessControl = new std::string;
        auto tessEval = new std::string;
        auto compute = new std::string;
        std::vector<std::string> splitData;
        std::string _data(fileData);
        std::string buffer;
        for (char letter : _data) {
            if (letter == '\n' || letter == '\r') {
                splitData.emplace_back(buffer);
                buffer.clear();
            } else buffer += letter;
        }
        splitData.emplace_back(buffer);
        for (auto &i : splitData) {
            char *line = const_cast<char *>(i.data());
            if (strstr(line, "#vertex") != nullptr) {
                type = 0;
            } else if (strstr(line, "#fragment") != nullptr) {
                type = 1;
            } else if (strstr(line, "#geometry") != nullptr) {
                type = 2;
            } else if (strstr(line, "#tessControl") != nullptr) {
                type = 3;
            } else if (strstr(line, "#tessEval") != nullptr) {
                type = 4;
            } else if (strstr(line, "#compute") != nullptr) {
                type = 5;
            } else {
                switch (type) {
                    case -1: {
                        errors.unknownType = true;
                    }
                    case 0: {
                        vertex->append(line);
                        *vertex += "\n";
                        break;
                    }
                    case 1: {
                        fragment->append(line);
                        *fragment += "\n";
                        break;
                    }
                    case 2: {
                        geometry->append(line);
                        *geometry += "\n";
                        break;
                    }
                    case 3: {
                        tessControl->append(line);
                        *tessControl += "\n";
                        break;
                    }
                    case 4: {
                        tessEval->append(line);
                        *tessEval += "\n";
                        break;
                    }
                    case 5: {
                        compute->append(line);
                        *compute += "\n";
                        break;
                    }
                }
            }
        }
        if (!hasError()) {
            if (vertID != -1)
                glDetachShader(programID, vertID);
            if (fragID != -1)
                glDetachShader(programID, fragID);
            if (geoID != -1)
                glDetachShader(programID, geoID);
            if (tessCID != -1)
                glDetachShader(programID, tessCID);
            if (tessEID != -1)
                glDetachShader(programID, tessEID);
            if (computeID != -1)
                glDetachShader(programID, computeID);
            glDeleteProgram(programID);
            locations.clear();
            programID = glCreateProgram();
            glUseProgram(programID);
            if (!vertex->empty()) {
                vertID = compileShader(GL_VERTEX_SHADER, vertex->data());
                glAttachShader(programID, vertID);
            }
            if (!fragment->empty()) {
                fragID = compileShader(GL_FRAGMENT_SHADER, fragment->data());
                glAttachShader(programID, fragID);
            }
            if (!geometry->empty()) {
                geoID = compileShader(GL_GEOMETRY_SHADER, geometry->data());
                glAttachShader(programID, geoID);
            }
            if (!tessControl->empty()) {
                tessCID = compileShader(GL_TESS_CONTROL_SHADER, tessControl->data());
                glAttachShader(programID, tessCID);
            }
            if (!tessEval->empty()) {
                tessEID = compileShader(GL_TESS_EVALUATION_SHADER, tessEval->data());
                glAttachShader(programID, tessEID);
            }
            if (!compute->empty()) {
                computeID = compileShader(GL_COMPUTE_SHADER, compute->data());
                glAttachShader(programID, computeID);
            }
            if (!hasError()) {
                glLinkProgram(programID);
                glValidateProgram(programID);
                glUseProgram(programID);
            }
            delete vertex;
            delete fragment;
            delete geometry;
            delete tessControl;
            delete tessEval;
            delete compute;
        }
    }
}

void Shader::addUniforms(std::vector<std::string> names) {
    for (auto name : names) {
        if (!locations.count(name)) {
            int id = glGetUniformLocation(programID, name.data());
            locations.insert(std::pair<std::string, int>(name, id));
        }
    }
}

void Shader::addUniform(std::string name) {
    if (!locations.count(name)) {
        int id = glGetUniformLocation(programID, name.data());
        locations.insert(std::pair<std::string, int>(name, id));
    }
}

int Shader::getUniform(const char *name) {
    return locations.at(name);
}

void Shader::bindAttribute(const char *name, unsigned int attribute) const {
    glBindAttribLocation(programID, attribute, name);
}

void Shader::setUniformBool(const char *name, const bool &value) {
    glUniform1i(getUniform(name), (int) value);
}

void Shader::setUniformLocation(const char *name, const int &value) {
    glUniform1i(getUniform(name), value);
}

void Shader::setUniform1i(const char *name, const int &value) {
    glUniform1i(getUniform(name), value);
}

void Shader::setUniform1iv(const char *name, const int &size, const int *data) {
    glUniform1iv(getUniform(name), size, data);
}

void Shader::setUniform2i(const char *name, const int &x, const int &y) {
    glUniform2i(getUniform(name), x, y);
}

void Shader::setUniform3i(const char *name, const int &x, const int &y, const int &z) {
    glUniform3i(getUniform(name), x, y, z);
}

void Shader::setUniform4i(const char *name, const int &x, const int &y, const int &z, const int &w) {
    glUniform4i(getUniform(name), x, y, z, w);
}

void Shader::setUniform1f(const char *name, const float &value) {
    glUniform1f(getUniform(name), value);
}

void Shader::setUniform1fv(const char *name, const int &size, const float *data) {
    glUniform1fv(getUniform(name), size, data);
}

void Shader::setUniform2f(const char *name, const float &x, const float &y) {
    glUniform2f(getUniform(name), x, y);
}

void Shader::setUniform2fv(const char *name, const int &size, const float *data) {
    glUniform2fv(getUniform(name), size, data);
}

void Shader::setUniform3f(const char *name, const float &x, const float &y, const float &z) {
    glUniform3f(getUniform(name), x, y, z);
}

void Shader::setUniform4f(const char *name, const float &x, const float &y, const float &z, const float &w) {
    glUniform4f(getUniform(name), x, y, z, w);
}

void Shader::setUniformMatrix4f(const char *name, const float *matrix, int count) {
    glUniformMatrix4fv(getUniform(name), count, GL_FALSE, matrix);
}

void Shader::setUniformMatrix4f(const char *name, const double *matrix, int count) {
    float temp[16] = {};
    for (int i = 0; i < 16; i++)
        temp[i] = float(matrix[i]);
    glUniformMatrix4fv(getUniform(name), count, GL_FALSE, temp);
}

void Shader::setUniformMatrix4d(const char *name, const double *matrix, int count) {
    glUniformMatrix4dv(getUniform(name), count, GL_FALSE, matrix);
}

Shader::~Shader() {
    glUseProgram(0);
    if (vertID != -1)
        glDetachShader(programID, vertID);
    if (fragID != -1)
        glDetachShader(programID, fragID);
    if (geoID != -1)
        glDetachShader(programID, geoID);
    if (tessCID != -1)
        glDetachShader(programID, tessCID);
    if (tessEID != -1)
        glDetachShader(programID, tessEID);
    if (computeID != -1)
        glDetachShader(programID, computeID);
    glDeleteProgram(programID);
    locations.clear();
}
