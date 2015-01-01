#include "ObjDeserializer.h"

Scene3D ObjDeserializer::ParseFile(const std::string& filename) const
{
    Scene3D result;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        if (line[0] == 'v')
            result.AddPoint(ParseVertex(line));
        if (line[0] == 'f')
        {
            Triangle3D parsedTriangle = ParseTriangle(line);
            result.AddTriangle(parsedTriangle);
        }
    }

    file.close();

    result.RecalculateNormals();
    return result;
}

Point ObjDeserializer::ParseVertex(const std::string& vertexInfo) const
{
    std::stringstream stream(vertexInfo);
    std::string v, x, y, z;

    stream >> v >> x >> y >> z;

    return Point(std::stod(x), std::stod(y), std::stod(z));
}

Triangle3D ObjDeserializer::ParseTriangle(const std::string& triangleInfo) const
{
    std::stringstream stream(triangleInfo);
    std::string f, p1, p2, p3;

    stream >> f >> p1 >> p2 >> p3;

    return Triangle3D(std::stoi(p1) - 1, std::stoi(p2) - 1, std::stoi(p3) - 1);
}
