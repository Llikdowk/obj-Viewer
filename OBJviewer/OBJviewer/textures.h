#pragma once
#include <GL\freeglut.h>
#include <vector>
#include "Image.h"

namespace texture {
    namespace {
        std::vector<GLuint> textures;
        GLuint model_texture;
    }

    static void registerTexture(unsigned int texture) {
        textures.push_back(texture);
    }

    static void apply_lights() {
        for (int i = 0; i < textures.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
    }

    static void erase_lights() {
        for (int i = 0; i < textures.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        }
    }

    static void typicalSettings() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    static void model() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, model_texture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // temp
        typicalSettings();
    }

    static void defineTexture(char* path, GLuint& texture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        img::loadImageFile(path);
    }

    static void init() {
        defineTexture("resources/mercedes/mercedes.jpg", model_texture);
        registerTexture(model_texture);
    }
}