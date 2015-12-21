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


    static void typicalSettings() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    static void model() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, model_texture);
        typicalSettings();
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    }

    static void defineTexture(char* path, GLuint& texture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        img::loadImageFile(path);
    }

    static void load(char* path) {
        defineTexture(path, model_texture);
        registerTexture(model_texture);
    }
}