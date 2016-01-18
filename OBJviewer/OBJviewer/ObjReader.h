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
        std::string material_name;
        std::vector<vec3> vertices;
        std::vector<vec3> uvs;
        std::vector<vec3> normals;
        bool hasNormals = true;
        bool hasTexture = true;
    };

    ObjReader(char* path);
    void createModel();
    void changeModel(char* path);

    const MtlReader::m_def getMaterialInfo(MtlReader::m_name);
    std::vector<node> mats;
    bool hasNormals = true;
    bool hasTexture = true;
    std::string path;


private:
    MtlReader mtl;

    std::vector< long > vertexIndices, uvIndices, normalIndices;
    std::vector< vec3 > vertexValues, uvValues, normalValues;

    void readObj();
    void clear();

    void parse_vt(std::string line, int linenum);
    void parse_vn(std::string line, int linenum);
    bool loadValues(const std::vector<long> &indices, const std::vector<vec3> &values, std::vector<vec3> &out);
    void process_current();
    void triangularize();
};