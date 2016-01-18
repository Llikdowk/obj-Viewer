#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "GLVector3f.h"
#include "MtlReader.h"

class ObjReader {
    
public:

    ObjReader(char* path);
    void createModel();
    void changeModel(char* path);
    int size();

    std::vector<vec3> vertices;
    std::vector<vec3> uvs; // preferred: Vec2
    std::vector<vec3> normals;

    bool quads = false;
    bool hasNormals = true;
    bool hasTexture = true;

private:
    MtlReader mtl;
    std::vector< long > vertexIndices, uvIndices, normalIndices;
    std::vector< vec3 > vertexValues, uvValues, normalValues;

    std::string path;
    void readObj();
    void clear();

    void parse_v(std::string line, int linenum);
    void parse_vt(std::string line, int linenum);
    void parse_vn(std::string line, int linenum);
    void parse_f(std::string line, int linenum);

    void triangularize();
};