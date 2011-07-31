
#pragma once

#include <glew.h>

#include "math.h"

public void glVertexf(Vec4f p);
public void glVertexi(Vec4i p);
public void glTranslatev(Vec4f p);

public GLuint emptyTexture(int width, int height);
public GLuint loadTexture(const char* filename);


