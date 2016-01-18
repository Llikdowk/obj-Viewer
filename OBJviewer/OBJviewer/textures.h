#pragma once
#include <GL\freeglut.h>
#include <vector>
#include <map>
#include "Image.h"

namespace texture {
    namespace {
        std::map<std::string, GLuint> textures;
        std::vector<GLuint> texture_pool(10);
    }

    static void typicalSettings() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    static void loadTexture(std::string path) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures.at(path));
        typicalSettings();
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    }

    static void defineTexture(std::string path, GLuint& texture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        char *cstr = &path[0u];
        img::loadImageFile(cstr);
    }

    static void precharge(std::string folder_path, std::string texture_path) {
        defineTexture(folder_path + texture_path, texture_pool.back());
        textures[texture_path] = texture_pool.back();
        texture_pool.push_back(GLuint());
        
    }
}