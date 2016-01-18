#include <regex>
#include "ObjReader.h"
#include "ErrorDefinitions.h"

inline bool exists_file(const std::string& name) {
    std::ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    }
    else {
        f.close();
        return false;
    }
}

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

bool ObjReader::loadValues(const std::vector<long> &indices, const std::vector<vec3> &values, std::vector<vec3> &out) {
    if (values.size() > 0) {
        for (int i = 0; i < indices.size(); ++i) {
            long index = indices[i];
            //std::cout << index - 1 << " : " << values[index-1].x << ", " << values[index - 1].y << ", " << values[index - 1].z << " len: " << values.size() << std::endl;
            vec3 value = values[index - 1];
            out.push_back(value);
        }
        return true;
    }
    else {
        return false;
    }
}

void ObjReader::createModel() {
    triangularize();
    readObj();
    bool texture = false;
    bool normals = false;
    for (auto it = materials.begin(); it != materials.end(); ++it) {
        if (texture == true && normals == true) break;
        
        texture = it->hasTexture || texture;
        normals = it->hasNormals || normals;
    }
    hasTexture = texture;
    hasNormals = normals;
    if (!texture) std::cout << "MODEL HAS NO TEXTURE COORDINATES" << std::endl;
    if (!normals) std::cout << "MODEL HAS NO NORMALS" << std::endl;
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
    if (exists_file(newFilePath)) {
        return;
    }
    myfile.open(newFilePath);
    int numLines = 0;
    std::ifstream in(path);
    std::string unused;
    while (std::getline(in, unused))
        ++numLines;

    int progress = 0;
    int last_change = 0;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("f ((\\+|-)?[[:digit:]]+\/(\\+|-)?[[:digit:]]+\/(\\+|-)?[[:digit:]]+ ){4}"))) { // v/vt/vn
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
        else if (std::regex_match(line, std::regex("f ((\\+|-)?[[:digit:]]+\/+(\\+|-)?[[:digit:]]+ ){4}"))) { // v/vt
            std::string sep = "/";
            if (line.find("//") != std::string::npos) {
                sep = "//";
            }
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
                f1 = "f " + std::to_string(v[0]) + sep + std::to_string(v[1]) + ' ';
                f1 += std::to_string(v[2]) + sep + std::to_string(v[3]) + ' ';
                f1 += std::to_string(v[4]) + sep + std::to_string(v[5]) + ' ';

                std::string f2;
                f2 = "f " + std::to_string(v[0]) + sep + std::to_string(v[1]) + ' ';
                f2 += std::to_string(v[4]) + sep + std::to_string(v[5]) + ' ';
                f2 += std::to_string(v[6]) + sep + std::to_string(v[7]) + ' ';

                myfile << f1 + '\n';
                myfile << f2 + '\n';
            }   
        }
        else {
            myfile << line + '\n';
            //std::cout << (float) progress / numLines * 100 << "              \r";
        }
        ++progress;
    }
    myfile.close();
    std::cout << "*TRIANGULARIZATION DONE*" << std::endl;
}

void ObjReader::process_current() {
    if (vertexIndices.size() > 0) {
        loadValues(vertexIndices, vertexValues, materials.back().vertices);
        materials.back().hasTexture = loadValues(uvIndices, uvValues, materials.back().uvs);
        materials.back().hasNormals = loadValues(normalIndices, normalValues, materials.back().normals);

        vertexIndices.clear();
        uvIndices.clear();
        normalIndices.clear();
    }
}

void ObjReader::readObj() {
    static bool debug = false;
    std::string newPath = path;
    newPath += "trian";
    std::ifstream file(newPath);

    if (!file) fileNotFoundError(path);

    unsigned int linenum = 0;
    unsigned long v_index = 1;
    unsigned long vt_index = 1;
    unsigned long vn_index = 1;

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
            int vertexIndex[3], uvIndex[3], normalIndex[3];

            std::string lineaux = line.substr(1, line.size());
            std::replace(lineaux.begin(), lineaux.end(), '/', ' ');
            std::istringstream iss(lineaux);
            if (iss // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                  >> vertexIndex[0] >> uvIndex[0] >> normalIndex[0]
                  >> vertexIndex[1] >> uvIndex[1] >> normalIndex[1]
                  >> vertexIndex[2] >> uvIndex[2] >> normalIndex[2]
                  ) { 
            }
            else {
                std::string lineaux = line.substr(1, line.size());
                std::replace(lineaux.begin(), lineaux.end(), '/', ' ');
                std::istringstream iss(lineaux);

                if (std::regex_match(line, std::regex("f ((\\+|-)?[[:digit:]]+\/\/(\\+|-)?[[:digit:]]+ ){3}"))) { // v//vn
                    if ((iss
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
                else { // v/vt
                    if ((iss
                         >> vertexIndex[0] >> uvIndex[0]
                         >> vertexIndex[1] >> uvIndex[1]
                         >> vertexIndex[2] >> uvIndex[2]
                         )
                        ) {
                    }
                    else {
                        parsingError("f", linenum);
                    }
                }
            }
               

            if (vertexIndex[0] >= 0) vertexIndices.push_back(vertexIndex[0]);
            else vertexIndices.push_back(v_index + vertexIndex[0]);

            if (vertexIndex[1] >= 0) vertexIndices.push_back(vertexIndex[1]);
            else vertexIndices.push_back(v_index + vertexIndex[1]);

            if (vertexIndex[2] >= 0) vertexIndices.push_back(vertexIndex[2]);
            else vertexIndices.push_back(v_index + vertexIndex[2]);


            if (uvIndex[0] >= 0) uvIndices.push_back(uvIndex[0]);
            else uvIndices.push_back(vt_index + uvIndex[0]);

            if (uvIndex[1] >= 0) uvIndices.push_back(uvIndex[1]);
            else uvIndices.push_back(vt_index + uvIndex[1]);

            if (uvIndex[2] >= 0) uvIndices.push_back(uvIndex[2]);
            else uvIndices.push_back(vt_index + uvIndex[2]);


            if (normalIndex[0] >= 0) normalIndices.push_back(normalIndex[0]);
            else normalIndices.push_back(vn_index + normalIndex[0]);

            if (normalIndex[1] >= 0) normalIndices.push_back(normalIndex[1]);
            else normalIndices.push_back(vn_index + normalIndex[1]);

            if (normalIndex[2] >= 0) normalIndices.push_back(normalIndex[2]);
            else normalIndices.push_back(vn_index + normalIndex[2]);  
        }
        else if (line.substr(0, 6) == "mtllib"){
            process_current();
            std::string mtl_name = line.substr(7, line.size());
            try {
                std::string current_folder = path.substr(0, path.rfind('/')+1);
                mtl.readMtl(current_folder + mtl_name);
            } catch (const std::invalid_argument& e) {
                std::cerr << e.what() << ": No materials defined" << std::endl;
            }
        }
        else if (line.substr(0, 6) == "usemtl") {
            struct node n;
            n.material_name = line.substr(7, line.size());
            materials.push_back(n);
        }

    }
    if (materials.size() == 0) {
        materials.push_back(node());
    }
    process_current();
    // debug:
    // std::cout << "SIZES: " << vertexIndices.size() << " " << uvIndices.size() << " " << normalIndices.size() << std::endl;
}

const MtlReader::m_def ObjReader::getMaterialInfo(MtlReader::m_name name) {
    MtlReader::m_def res;
    try {
        res = mtl.materials.at(name);
    }
    catch (std::out_of_range e) {
        std::cerr << "No mtllib file found in *.obj file." << std::endl;
    }
    return res;
}
