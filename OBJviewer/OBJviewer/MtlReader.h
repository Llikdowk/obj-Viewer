#pragma once
#include "GLVector3f.h"
#include "ErrorDefinitions.h"
#include <map>
#include <fstream>
#include <sstream>
#include <regex>

class MtlReader {
public:
    typedef std::string m_name;
    struct m_def {
        float Ns;
        float Ni;
        float d;
        float Tr;
        vec3 Tf;
        float illum;
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        vec3 Ke;
        std::string map_Ka;
        std::string map_Kd;
        std::string map_refl;

        void clear() {
            Ns = 0.0f; Ni = 0.0f; d = 0.0f; Tr = 0.0f; illum = 0.0f;
            map_Ka = ""; map_Kd = ""; map_refl = "";
            Tf = vec3(0, 0, 0);
            Ka = vec3(0, 0, 0);
            Kd = vec3(0, 0, 0);
            Ks = vec3(0, 0, 0);
            Ke = vec3(0, 0, 0);
        }
    };

    std::map<m_name, m_def> materials;
    
    void readMtl(std::string path) {
        std::ifstream file(path);
        if (!file) fileNotFoundError(path);
        std::string line;
        m_name key;
        m_def value;
        std::smatch match;
        while (std::getline(file, line)) {
            if (std::regex_search(line, match, std::regex("newmtl (.*)"))) {
                if (key.size() > 0) {
                    materials[key] = value;
                }
                key = match[1];
                value.clear();
            }
            else if (std::regex_search(line, match, std::regex("Ns ([0-9]+\\.?[0-9]*)"))) {
                value.Ns = std::stof(match[1]);
            }
            else if (std::regex_search(line, match, std::regex("Ni ([0-9]+\\.?[0-9]*)"))) {
                value.Ni = std::stof(match[1]);
            }
            else if (std::regex_search(line, match, std::regex("d ([0-9]+\\.?[0-9]*$)"))) {
                value.d = std::stof(match[1]);
            }
            else if (std::regex_search(line, match, std::regex("Tr ([0-9]+\\.?[0-9]*)"))) {
                value.Tr = std::stof(match[1]);
            }
            else if (std::regex_search(line, match, std::regex("Tf ([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]*)"))) {
                float x = std::stof(match[1]);
                float y = std::stof(match[2]);
                float z = std::stof(match[3]);
                value.Tf = vec3(x, y, z);
            }
            else if (std::regex_search(line, match, std::regex("illum ([0-9]+\\.?[0-9]*)"))) {
                value.illum = std::stof(match[1]);
            }
            else if (std::regex_search(line, match, std::regex("Ka ([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]*)"))) {
                float x = std::stof(match[1]);
                float y = std::stof(match[2]);
                float z = std::stof(match[3]);
                value.Ka = vec3(x, y, z);
            }
            else if (std::regex_search(line, match, std::regex("Kd ([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]*)"))) {
                float x = std::stof(match[1]);
                float y = std::stof(match[2]);
                float z = std::stof(match[3]);
                value.Kd = vec3(x, y, z);
            }
            else if (std::regex_search(line, match, std::regex("Ks ([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]*)"))) {
                float x = std::stof(match[1]);
                float y = std::stof(match[2]);
                float z = std::stof(match[3]);
                value.Ks = vec3(x, y, z);
            }
            else if (std::regex_search(line, match, std::regex("Ke ([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]* )([0-9]+\\.?[0-9]*)"))) {
                float x = std::stof(match[1]);
                float y = std::stof(match[2]);
                float z = std::stof(match[3]);
                value.Ke = vec3(x, y, z);
            }
            else if (std::regex_search(line, match, std::regex("map_Ka (.*)"))) {
                value.map_Ka = match[1];
            }
            else if (std::regex_search(line, match, std::regex("map_Kd (.*)"))) {
                value.map_Kd = match[1];
            }
            else if (std::regex_search(line, match, std::regex("map_refl (.*)"))) {
                value.map_refl = match[1];
            }
            
        }
        materials[key] = value;
    }
    
};