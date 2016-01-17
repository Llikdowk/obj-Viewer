#include "ObjReader.h"
#include <regex>


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
    //triangularize();
    std::cout << "DONE" << std::endl;
    readObj();
    loadValues(vertexIndices, vertexValues, vertices);
    loadValues(uvIndices, uvValues, uvs);
    loadValues(normalIndices, normalValues, normals);
}

void fileNotFoundError(std::string path) {
    std::string msg = "ERROR: The file " + path + " cannot be found";
    throw std::invalid_argument(msg);
}

void parsingError(std::string s, int linenum) {
    std::string msg = "ERROR: parsing " + s + " at line " + std::to_string(linenum);
    throw std::invalid_argument(msg);
}

void ObjReader::parse_vt(std::string line, int linenum) {
    GLVector3f::GLVector3f uv;
    line = line.substr(2, line.size());
    std::istringstream iss(line);
    if (!(iss >> uv.x >> uv.y)) {
        parsingError("vt", linenum);
    }
    uv.z = 0;
    //std::cout << uv.x << " " << uv.y << " " << uv.z << std::endl;
    uvValues.push_back(uv);
}

void ObjReader::parse_vn(std::string line, int linenum) {

    GLVector3f::GLVector3f normal;

    line = line.substr(2, line.size());
    std::istringstream iss(line);
    if (!(iss >> normal.x >> normal.y >> normal.z)) {
        parsingError("vn", linenum);
    }
    //std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
    normalValues.push_back(normal);
}

void ObjReader::triangularize() {
    std::ifstream file(path);
    std::string line;

    std::ofstream myfile;
    std::string newFilePath = path;
    newFilePath += "trian";
    myfile.open(newFilePath);
    //myfile << "Writing this to a file.\n";

    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("f ((\\+|-)?[[:digit:]]+\/(\\+|-)?[[:digit:]]+\/(\\+|-)?[[:digit:]]+ ){4}"))) {            
            int v[12];
            line = line.substr(1, line.size());
            std::replace(line.begin(), line.end(), '/', ' ');
            std::istringstream iss(line);
            if (iss // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                >> v[0] >> v[1] >> v[2]
                >> v[3] >> v[4] >> v[5]
                >> v[6] >> v[7] >> v[8]
                >> v[9] >> v[10] >> v[11]
                ) {
                std::string f1;
                f1  = "f " + std::to_string(v[0]) + '/' + std::to_string(v[1]) + '/' + std::to_string(v[2]) + ' ';
                f1 += std::to_string(v[3]) + '/' + std::to_string(v[4]) + '/' + std::to_string(v[5]) + ' ';
                f1 += std::to_string(v[6]) + '/' + std::to_string(v[7]) + '/' + std::to_string(v[8]) + ' ';

                std::string f2;
                f2  = "f " + std::to_string(v[0]) + '/' + std::to_string(v[1]) + '/' + std::to_string(v[2]) + ' ';
                f2 += std::to_string(v[6]) + '/' + std::to_string(v[7]) + '/' + std::to_string(v[8]) + ' ';
                f2 += std::to_string(v[9]) + '/' + std::to_string(v[10]) + '/' + std::to_string(v[11]) + ' ';

                myfile << f1 + '\n';
                myfile << f2 + '\n';
            }
        }
        else if (std::regex_match(line, std::regex("f ((\\+|-)?[[:digit:]]+\/+(\\+|-)?[[:digit:]]+ ){4}"))) {

            int v[8];
            line = line.substr(1, line.size());
            std::replace(line.begin(), line.end(), '/', ' ');
            std::istringstream iss(line);
            if (iss // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                >> v[0] >> v[1]
                >> v[2] >> v[3]
                >> v[4] >> v[5]
                >> v[6] >> v[7]
                ) {
                std::string f1;
                f1 = "f " + std::to_string(v[0]) + '/' + std::to_string(v[1]) + ' ';
                f1 += std::to_string(v[2]) + '/' + std::to_string(v[3]) + ' ';
                f1 += std::to_string(v[4]) + '/' + std::to_string(v[5]) + ' ';

                std::string f2;
                f2 = "f " + std::to_string(v[0]) + '/' + std::to_string(v[1]) + ' ';
                f2 += std::to_string(v[4]) + '/' + std::to_string(v[5]) + ' ';
                f2 += std::to_string(v[6]) + '/' + std::to_string(v[7]) + ' ';

                myfile << f1 + '\n';
                myfile << f2 + '\n';
            }
        }
        else {
            myfile << line + '\n';
        }
    }
    myfile.close();
}

void ObjReader::readObj() {
    static bool debug = false;
    std::ifstream file(path);

    if (!file) fileNotFoundError(path);

    int linenum = 0;
    int v_index = 1;
    int vt_index = 1;
    int vn_index = 1;

    std::string line;
    while (std::getline(file, line)) {

        ++linenum;
        if (debug) std::cout << linenum << " " << line << std::endl;
        if (line.size() == 0) continue;
        
        bool is_vt = line[0] == 'v' && line[1] == 't';
        if (is_vt) {
            parse_vt(line, linenum);
            ++vt_index;
            continue;
        }
        
        bool is_vn = line[0] == 'v' && line[1] == 'n';
        if (is_vn) {
            parse_vn(line, linenum);
            ++vn_index;
            continue;
        }
        //v
        else if (line[0] == 'v') {
            GLVector3f::GLVector3f vertex;

            line = line.substr(1, line.size());
            std::istringstream iss(line);
            if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
                parsingError("v", linenum);
            }
            //std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
            vertexValues.push_back(vertex);
            ++v_index;

        }
        //f
        else if (line[0] == 'f') {
            int vertexIndex[4], uvIndex[4], normalIndex[4];
            int x, y, z;
            line = line.substr(1, line.size());
            std::replace(line.begin(), line.end(), '/', ' ');
            std::istringstream iss(line);
            if (iss // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                  >> vertexIndex[0] >> uvIndex[0] >> normalIndex[0]
                  >> vertexIndex[1] >> uvIndex[1] >> normalIndex[1]
                  >> vertexIndex[2] >> uvIndex[2] >> normalIndex[2]
                  ) { 
                //if (iss >> vertexIndex[3] >> uvIndex[3] >> normalIndex[3]) {
                //    quads = true;
                //}
            }
            else {
                std::istringstream iss(line);
                if ((iss // f v1//vn1 v2//vn2 v3//vn3
                      >> vertexIndex[0] >> normalIndex[0]
                      >> vertexIndex[1] >> normalIndex[1]
                      >> vertexIndex[2] >> normalIndex[2]
                      )
                    ) {
                }
                else {
                    parsingError("f", linenum);
                }
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

            if (normalIndex[1] > 0) normalIndices.push_back(normalIndex[1]);
            else normalIndices.push_back(vn_index + normalIndex[1]);

            if (normalIndex[2] > 0) normalIndices.push_back(normalIndex[2]);
            else normalIndices.push_back(vn_index + normalIndex[2]);  
        }
        else if (line.substr(0, 6) == "mtllib"){
            mtl_path = line.substr(7, line.size());
            //load_mtl();
        }

    }
    //std::cout << "SIZES: " << vertexIndices.size() << " " << uvIndices.size() << " " << normalIndices.size() << std::endl;
}
