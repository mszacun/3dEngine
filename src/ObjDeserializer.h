#ifndef OBJ_DESERIALIZER_H
#define OBJ_DESERIALIZER_H

#include <sstream>
#include <fstream>
#include "Scene.h"
#include "Triangle.h"

struct ObjFile
{
    ScenePtr scene;
    Vector cameraPosition;
};

class ObjDeserializer
{
    public:
        ObjFile ParseFile(const std::string& filename) const;

        Vector ParseVertex(const std::string& vertexInfo) const;
        void ParseTextureCoordinates(const std::string& textureInfo,
                ScenePtr scene, int triangleNumber) const;
        Triangle3D ParseTriangle(const std::string& triangleInfo) const;

};

class ObjSerializer
{
    public:
        void SaveToFile(const std::string& path, const ObjFile& objFile);

        void SavePoint(const Vector& vertex, const std::string& prefix);
        void SaveTriangle(const Triangle3D& t);
        void SaveTexture(const Triangle3D& t);

    private:
        std::ofstream file_;
};

#endif
