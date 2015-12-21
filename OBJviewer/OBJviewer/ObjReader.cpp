#include "ObjReader.h"
//typedef GLVector3f::GLVector3f vec3;

ObjReader::ObjReader(char* path) : path(path) {
}

void ObjReader::changeModel(char* path) {
    this->path = path;
    clear();
}

void ObjReader::clear() {
    vertexIndices.clear();
    uvIndices.clear();
    normalIndices.clear();
    vertexValues.clear();
    uvValues.clear();
    normalValues.clear();
}

int ObjReader::size() {
    return vertices.size();
}

void loadValues(const std::vector<int> &indices, const std::vector<vec3> &values, std::vector<vec3> &out) {
    for (int i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        vec3 value = values[index - 1];
        out.push_back(value);
    }
}

void ObjReader::createModel() {
    readObj();
    loadValues(vertexIndices, vertexValues, vertices);
    loadValues(uvIndices, uvValues, uvs);
    loadValues(normalIndices, normalValues, normals);
}

bool ObjReader::readObj() {
    static bool debug = false;
    std::ifstream file(path);

    if (!file) {
        std::cerr << "ERROR! The path " << path << "cannot be found";
        return false;
    }
    int linenum = 0;
    int v_index = 1;
    int vt_index = 1;
    int vn_index = 1;

    std::string line;
    while (std::getline(file, line)) {
        ++linenum;

        if (debug) {
            std::cout << linenum << " " << line << std::endl;
        }
        if (line.size() == 0) continue;
        if (line[0] == 'v' && line[1] == 't') {

            GLVector3f::GLVector3f uv;
            line = line.substr(2, line.size());
            std::istringstream iss(line);
            if (!(iss >> uv.x >> uv.y)) {
                std::cerr << "ERROR parsing vt at line " << linenum << std::endl;
                break;
            }
            uv.z = 0;
            //std::cout << uv.x << " " << uv.y << " " << uv.z << std::endl;
            uvValues.push_back(uv);
            ++vt_index;
        }
        else if (line[0] == 'v' && line[1] == 'n') {

            GLVector3f::GLVector3f normal;

            line = line.substr(2, line.size());
            std::istringstream iss(line);
            if (!(iss >> normal.x >> normal.y >> normal.z)) {
                std::cerr << "ERROR parsing vn at line " << linenum << std::endl;
                break;
            }
            //std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
            normalValues.push_back(normal);
            ++vn_index;
        }
        else if (line[0] == 'v') {
            GLVector3f::GLVector3f vertex;

            line = line.substr(1, line.size());
            std::istringstream iss(line);
            if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
                std::cerr << "ERROR parsing v at line " << linenum << std::endl;
                break;
            }
            //std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
            vertexValues.push_back(vertex);
            ++v_index;

        }
        else if (line[0] == 'f') { // TODO: faces like-> f v1//vn1 v2//vn2 v3//vn3 ...
            int vertexIndex[3], uvIndex[3], normalIndex[3];

            line = line.substr(1, line.size());
            std::replace(line.begin(), line.end(), '/', ' ');
            std::istringstream iss(line);
            if (!(iss >> vertexIndex[0] >> uvIndex[0] >> normalIndex[0] >> vertexIndex[1] >> uvIndex[1] >> normalIndex[1] >> vertexIndex[2] >> uvIndex[2] >> normalIndex[2])) {
                std::cerr << "ERROR parsing f at line " << linenum << std::endl;
                break;
            }

            if (vertexIndex[0] > 0) vertexIndices.push_back(vertexIndex[0]);
            else vertexIndices.push_back(v_index + vertexIndex[0]);

            if (vertexIndex[1] > 0) vertexIndices.push_back(vertexIndex[1]);
            else vertexIndices.push_back(v_index + vertexIndex[1]);

            if (vertexIndex[2] > 0) vertexIndices.push_back(vertexIndex[2]);
            else vertexIndices.push_back(v_index + vertexIndex[2]);


            if (uvIndex[0] > 0) uvIndices.push_back(uvIndex[0]);
            else uvIndices.push_back(vt_index + uvIndex[0]);

            if (uvIndex[1] > 0) uvIndices.push_back(uvIndex[1]);
            else uvIndices.push_back(vt_index + uvIndex[1]);

            if (uvIndex[2] > 0) uvIndices.push_back(uvIndex[2]);
            else uvIndices.push_back(vt_index + uvIndex[2]);


            if (normalIndex[0] > 0) normalIndices.push_back(normalIndex[0]);
            else normalIndices.push_back(vn_index + normalIndex[0]);

            if (normalIndex[0] > 0) normalIndices.push_back(normalIndex[1]);
            else normalIndices.push_back(vn_index + normalIndex[1]);

            if (normalIndex[0] > 0) normalIndices.push_back(normalIndex[2]);
            else normalIndices.push_back(vn_index + normalIndex[2]);

        }

    }
    //std::cout << "SIZES: " << vertexIndices.size() << " " << uvIndices.size() << " " << normalIndices.size() << std::endl;

    return true;
}
