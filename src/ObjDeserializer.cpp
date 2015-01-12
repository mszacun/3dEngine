#include "ObjDeserializer.h"

ObjFile ObjDeserializer::ParseFile(const std::string& filename) const
{
    Scene3D result;
    std::ifstream file(filename);
    std::string line;
    Vector cameraPosition(0, 0, -10);

    while (std::getline(file, line))
    {
        if (line[0] == 'v')
            result.AddPoint(ParseVertex(line));
        if (line[0] == 'f')
        {
            Triangle3D parsedTriangle = ParseTriangle(line);
            result.AddTriangle(parsedTriangle);
        }
        if (line[0] == 'c')
            cameraPosition = ParseVertex(line);
    }

    file.close();

    result.RecalculateNormals();
    return { result, cameraPosition };
}

Vector ObjDeserializer::ParseVertex(const std::string& vertexInfo) const
{
    std::stringstream stream(vertexInfo);
    std::string v;
    double x, y, z;

    stream >> v >> x >> y >> z;

    return Vector(x, y, z);
}

Triangle3D ObjDeserializer::ParseTriangle(const std::string& triangleInfo) const
{
    std::stringstream stream(triangleInfo);
    std::string f, p1, p2, p3;

    stream >> f >> p1 >> p2 >> p3;

    return Triangle3D(std::stoi(p1) - 1, std::stoi(p2) - 1, std::stoi(p3) - 1);
}

void ObjSerializer::SaveToFile(const std::string& path, const ObjFile& objFile)
{
    file_.open(path);

    for (const Vector& v : objFile.scene.GetPoints())
        SavePoint(v, "v");

    for (const Triangle3D& t : objFile.scene.GetTriangles())
        SaveTriangle(t);

    SavePoint(objFile.cameraPosition, "c");

    file_.close();
}

void ObjSerializer::SavePoint(const Vector& vertex, const std::string& prefix)
{
    file_ << prefix << " " << vertex.GetX() << " " << vertex.GetY() << " " << vertex.GetZ() << std::endl;
}

void ObjSerializer::SaveTriangle(const Triangle3D& t)
{
    file_ << "f " << t.GetP1() + 1 << " " << t.GetP2() + 1 << " " << t.GetP3() + 1 << std::endl;
}
