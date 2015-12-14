#include "ObjReader.h"

ObjReader::ObjReader(char* path) : path(path) {
    loadObj();
}

int ObjReader::size() {
    return vertices.size();
}

void ObjReader::loadObj() {
    readObj();

    for (int i = 0; i < vertexIndices.size(); i++) {

        int vertexIndex = vertexIndices[i];
        GLVector3f::GLVector3f vertex = verticesValues[vertexIndex - 1];
        vertices.push_back(vertex);

        //std::cout << i << " " << vertexIndices[i] << std::endl;
        int uvIndex = uvIndices[i];
        GLVector3f::GLVector3f uv = uvsValues[uvIndex - 1];
        uvs.push_back(uv);


        int normalIndex = normalIndices[i];
        GLVector3f::GLVector3f normal = normalsValues[normalIndex - 1];
        normals.push_back(normal);
    }
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
            uvsValues.push_back(uv);
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
            normalsValues.push_back(normal);
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
            verticesValues.push_back(vertex);
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
