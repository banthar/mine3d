
#include "config.h"

#include "utils.h"

#include "math.h"

#include "glew.h"
#include "SDL.h"
#include "SDL_image.h"

public GLuint emptyTexture(int width, int height)
{
	
	int data[width*height];
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		 0,
		 GL_RGBA,
		 width,height,
		 0,
		 GL_RGBA,
		 GL_UNSIGNED_BYTE,
		 data);

	return texture;
		 
}

public GLuint loadTexture(const char* filename)
{

    SDL_Surface *surface=IMG_Load(filename);

    if ( surface == NULL ) {
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,
             0,
             GL_RGBA,
             surface->w, surface->h,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             surface->pixels);

    SDL_FreeSurface(surface);

	return texture;

}

