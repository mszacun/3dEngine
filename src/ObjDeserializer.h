#ifndef OBJ_DESERIALIZER_H
#define OBJ_DESERIALIZER_H

#include <sstream>
#include <fstream>
#include "Scene.h"
#include "Triangle.h"

struct ObjFile
{
    Scene3D scene;
    Vector cameraPosition;
};

class ObjDeserializer
{
    public:
        ObjFile ParseFile(const std::string& filename) const;

        Vector ParseVertex(const std::string& vertexInfo) const;
        Triangle3D ParseTriangle(const std::string& triangleInfo) const;

};

#endif
