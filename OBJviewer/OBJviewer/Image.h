#pragma once
#include <freeimage\FreeImage.h>
#include <GL/glui.h>
#include <GL\glext.h>
#include <iostream>

namespace img {
    static void loadImageFile(char* nombre)
    {
        FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(nombre, 0);
        FIBITMAP* imagen = FreeImage_Load(formato, nombre);
        if (imagen) {
            FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);
            // Lectura de dimensiones y colores
            int w = FreeImage_GetWidth(imagen32b);
            int h = FreeImage_GetHeight(imagen32b);
            GLubyte* texeles = FreeImage_GetBits(imagen32b);
            // Carga como textura actual
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, texeles);
            // Liberar recursos
            FreeImage_Unload(imagen);
            FreeImage_Unload(imagen32b);
        }
        else {
            std::cerr << "ERROR: file " << nombre << " could not be loaded" << std::endl;
        }
    }

    static void saveScreenshot(char* nombre, int ancho, int alto) {
        int pix = ancho * alto;
        BYTE *pixels = new BYTE[3 * pix];
        glReadBuffer(GL_FRONT);
        glReadPixels(0, 0, ancho, alto, GL_BGR, GL_UNSIGNED_BYTE, pixels);
        FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, ancho, alto,
                                                     ancho * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
        FreeImage_Save(FIF_PNG, img, nombre, 0);
        delete pixels;
    }
}