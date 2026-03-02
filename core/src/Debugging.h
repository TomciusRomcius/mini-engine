//
// Created by tomas on 2/26/26.
//

#pragma once
#include <GL/glew.h>

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define glCall(expression) expression; glCheckError_(__FILE__, __LINE__);