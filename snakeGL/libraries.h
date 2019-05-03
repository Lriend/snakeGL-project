#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <queue>
#include <thread>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL2.h>

#include "vertex.h"
#include "prefab.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "model.h"

enum TEXTURE { WHITE, FIELD, TAIL_RGB, TAIL_STRAIGHT, TAIL_TORIGHT, TAIL_TOLEFT, TAIL_END, RED, GRAY, BROWN, GREEN, BLACK, GOLDEN, BLUE, YELLOW, DARK };

enum SHADER { SHADER_CORE_PROGRAM };
enum MESH { MESH_PLANE, MESH_CUBE, MESH_FIELD };