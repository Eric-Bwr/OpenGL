#pragma once

#include "OpenGL.h"
#include <map>
#include <string>

class Shader{
public:
    struct Errors {
        bool failedToLocate = false;
        bool failedToAllocate = false;
        bool fileIsEmpty = false;
        bool unknownType = false;
        bool failedToCompileVertex = false;
        std::string vertexMessage;
        bool failedToCompileFragment = false;
        std::string fragmentMessage;
        bool failedToCompileGeometry = false;
        std::string geometryMessage;
        bool failedToCompileTessEval = false;
        std::string tessEvalMessage;
        bool failedToCompileTessControl = false;
        std::string tessControlMessage;
        bool failedToCompileCompute = false;
        std::string computeMessage;
    };
public:
    explicit Shader();
    explicit Shader(const char* path);
    void addFromFile(const char* path);
    void addVertexShader(const char *vertex);
    void addFragmentShader(const char *fragment);
    void addGeometryShader(const char* geometry);
    void addComputeShader(const char* compute);
    void addTessellationControlShader(const char *tessControl);
    void addTessellationEvaluationShader(const char *tessEval);
    void finish() const;
    void bind() const;
    static void unbind();
    void reload();
    void bindAttribute(const char* name, unsigned int attribute) const;
    void setUniformBool(const char* name, const bool& value);
    void setUniformLocation(const char* name, const int& value);
    void setUniform1i(const char* name, const int& value);
    void setUniform1iv(const char* name, const int& size, const int* data);
    void setUniform2i(const char* name, const int& x, const int& y);
    void setUniform3i(const char* name, const int& x, const int& y, const int& z);
    void setUniform4i(const char* name, const int& x, const int& y, const int& z, const int& w);
    void setUniform1f(const char* name, const float& value);
    void setUniform1fv(const char* name, const int& size, const float* data);
    void setUniform2f(const char* name, const float& x, const float& y);
    void setUniform2fv(const char* name, const int& size, const float* data);
    void setUniform3f(const char* name, const float& x, const float& y, const float& z);
    void setUniform4f(const char* name, const float& x, const float& y, const float& z, const float& w);
    void setUniformMatrix4f(const char* name, const float* matrix);
    inline unsigned int getProgramID() const { return programID; }
    inline Errors& getErrors(){ return errors; }
    bool hasError() const;
    std::string getErrorMessage();
    ~Shader();
private:
    const char* path;
    unsigned int programID;
    unsigned int vertID = -1;
    unsigned int fragID = -1;
    unsigned int geoID = -1;
    unsigned int tessCID = -1;
    unsigned int tessEID = -1;
    unsigned int computeID = -1;
    char* readFile(const char* filePath);
    std::map<const char*, unsigned int> locations;
    int getUniform(const char* name);
    unsigned int compileShader(unsigned int type, const char* source);
    Errors errors;
};