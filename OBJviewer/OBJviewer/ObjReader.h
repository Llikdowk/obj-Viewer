#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>

#include "GLVector3f.h"
#include "MtlReader.h"

class ObjReader {
    
public:
    struct node {
        std::string m_name;
        long start;
    };

    ObjReader(char* path);
    void createModel();
    void changeModel(char* path);
    int size();

    std::vector<vec3> vertices;
    std::vector<vec3> uvs; // preferred: Vec2
    std::vector<vec3> normals;
    std::vector<node> materials;

    bool quads = false;
    bool hasNormals = true;
    bool hasTexture = true;

    const MtlReader::m_def getMaterialInfo(MtlReader::m_name);

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
    bool ObjReader::loadValues(const std::vector<long> &indices, const std::vector<vec3> &values, std::vector<vec3> &out);

    void triangularize();
};