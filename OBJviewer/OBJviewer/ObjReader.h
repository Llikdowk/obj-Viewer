#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "GLVector3f.h"

class ObjReader {
public:
    ObjReader(char* path);
    void loadObj();
    int size();

    std::vector< GLVector3f::GLVector3f > vertices;
    std::vector< GLVector3f::GLVector3f > uvs; // preferred: Vec2
    std::vector< GLVector3f::GLVector3f > normals;


private:
    std::vector< int > vertexIndices, uvIndices, normalIndices;
    std::vector< GLVector3f::GLVector3f > verticesValues;
    std::vector< GLVector3f::GLVector3f > uvsValues; // preferred: Vec2
    std::vector< GLVector3f::GLVector3f > normalsValues;

    char* path;
    bool readObj();
};