#ifndef OBJ_DESERIALIZER_H
#define OBJ_DESERIALIZER_H

#include <sstream>
#include <fstream>
#include "Scene.h"
#include "Triangle.h"

class ObjDeserializer
{
    public:
        Scene3D ParseFile(const std::string& filename) const;

        Point ParseVertex(const std::string& vertexInfo) const;
        Triangle3D ParseTriangle(const std::string& triangleInfo) const;

};

#endif
