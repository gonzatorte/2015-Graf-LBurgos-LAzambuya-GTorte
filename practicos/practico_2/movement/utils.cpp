#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <cstdio>

#include <SDL/SDL.h>
#include <SDL_ttf.h>

#include "SDL/SDL_opengl.h"
#include <GL/glut.h>

void DrawTexto(TTF_Font * font, const char* text, GLuint * texture)
{
    SDL_Color color = {0,0,0};
    SDL_Surface * text_surface;
    text_surface = TTF_RenderText_Solid(font, text, color);
    if (text_surface == NULL)
    {
        throw "No pude cargar el texto";
    }
    int w = pow(2,ceil(log(text_surface->w)/log(2)));
    SDL_Surface * myNewSurface;
    myNewSurface = SDL_CreateRGBSurface(0,w,w,24,0xff000000,0x00ff0000,0x0000ff00,0);
    SDL_BlitSurface(text_surface,0,myNewSurface,0);
    SDL_FreeSurface(text_surface);

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, w, w, 0, GL_RGB, GL_UNSIGNED_BYTE, myNewSurface->pixels );
    //ToDo: No se si puedo hacerle free...
    SDL_FreeSurface(myNewSurface);
}

GLuint DrawBMP(SDL_Surface* bmp, int width, int height){
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    return 0;
}

GLuint raw_texture_load(const char *filename, int width, int height)
{
    GLuint texture;
    unsigned char *data;
    FILE *file;

    // open texture data
    file = fopen(filename, "rb");
    if (file == NULL) return 0;

    // allocate buffer
    data = (unsigned char*) malloc(width * height * 4);

    // read texture data
    fread(data, width * height * 4, 1, file);
    fclose(file);

    // allocate a texture name
    glGenTextures(1, &texture);

    // select our current texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture should tile
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // build our texture mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // free buffer
    free(data);

    return texture;
}

//int LoadBitmap(char *filename)
//{
//    FILE * file;
//    char temp;
//    long i;
//
//    BITMAPINFOHEADER infoheader;
//
//    num_texture++; // The counter of the current texture is increased
//
//    if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading
//
//    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
//    fread(&infoheader.biWidth, sizeof(int), 1, file);
//
//    fread(&infoheader.biHeight, sizeof(int), 1, file);
//
//    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
//    if (infoheader.biPlanes != 1) {
//	    printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
//	    return 0;
//    }
//
//    // read the bpp
//    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
//    if (infoheader.biBitCount != 24) {
//      printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
//      return 0;
//    }
//
//    fseek(file, 24, SEEK_CUR);
//
//    // read the data.
//    infoheader.data = (char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
//    if (infoheader.data == NULL) {
//	    printf("Error allocating memory for color-corrected image data\n");
//	    return 0;
//    }
//
//    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
//	    printf("Error reading image data from %s.\n", filename);
//	    return 0;
//    }
//
//    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
//	    temp = infoheader.data[i];
//	    infoheader.data[i] = infoheader.data[i+2];
//	    infoheader.data[i+2] = temp;
//    }
//
//
//    fclose(file); // Closes the file stream
//}
