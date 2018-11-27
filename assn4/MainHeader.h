#pragma once

#include <iostream>

#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <functional>

#include <ctime>
#include <sstream>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "Opengl32.lib")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "TVector.h"
#include "Pointer.h"


using namespace std;
using namespace glm;

typedef TVector<int, 2> T2Int;
typedef TVector<int, 3> T3Int;
typedef TVector<int, 4> T4Int;

typedef TVector<double, 4> T4Double;

typedef glm::ivec2 Vec2i;
typedef glm::ivec3 Vec3i;
typedef glm::ivec4 Vec4i;
typedef glm::vec2 Vec2d;
typedef glm::vec3 Vec3d;
typedef glm::vec4 Vec4d;


typedef int Type_Not_Determined;


#define PI 3.141592
#define DTR(x) ((x)*PI/180.0)
#define RTD(x) ((x)*180.0/PI)
#define MC_In(a,b,x) (x <= b && x >= a)
#define MC_R(x) ((int)((x)+0.5))
#define CR_(x) const x&




