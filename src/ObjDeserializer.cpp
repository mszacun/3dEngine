#include "ObjDeserializer.h"

ObjFile ObjDeserializer::ParseFile(const std::string& filename) const
{
    ScenePtr result = std::make_shared<Scene3D>();
    std::ifstream file(filename);
    std::string line;
    Vector cameraPosition(0, 0, -10);
    int textureTriangleNumber = 0;
    bool isTexturePresent = false;

    // default values
    result->SetLightColor(QColor("white"));
    result->SetAmbientLightColor(QColor("black"));
    result->SetLightPosition(Vector(30, -50, -4));

    while (std::getline(file, line))
    {
        if (line[0] == 'v')
            result->AddPoint(ParseVertex(line));
        if (line[0] == 'f')
        {
            Triangle3D parsedTriangle = ParseTriangle(line);
            result->AddTriangle(parsedTriangle);
        }
        if (line[0] == 'c')
            cameraPosition = ParseVertex(line);
        if (line[0] == 'p')
            ParseTextureCoordinates(line, result,  textureTriangleNumber++);
        if (line == "texture on")
            isTexturePresent = true;
    }

    file.close();

    MaterialPtr material;
    if (isTexturePresent)
    {
        QImage image;
        image.load("texture.jpg");
        material = std::make_shared<ImageTextureMaterial>(image);
    }
    else
        material = std::make_shared<Material>();
    result->SetMaterial(material);

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

void ObjDeserializer::ParseTextureCoordinates(const std::string& textureInfo, ScenePtr scene, int n) const
{
    std::stringstream stream(textureInfo);
    std::string t;
    double x1, y1, x2, y2, x3, y3;

    stream >> t >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    scene->AddTexture(n, Vector(x1, y1, 0), Vector(x2, y2, 0), Vector(x3, y3, 0));
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

    for (const Vector& v : objFile.scene->GetPoints())
        SavePoint(v, "v");

    for (const Triangle3D& t : objFile.scene->GetTriangles())
    {
        SaveTriangle(t);
        if (!objFile.scene->GetMaterial()->IsConstant())
            SaveTexture(t);
    }


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

void ObjSerializer::SaveTexture(const Triangle3D& t)
{
    file_ << "t " << t.GetP1TextureCoordinates().GetX() << " " <<  t.GetP1TextureCoordinates().GetY() << " " <<
        t.GetP2TextureCoordinates().GetX() << " " << t.GetP2TextureCoordinates().GetY() << " " <<
        t.GetP3TextureCoordinates().GetX() << " " << t.GetP3TextureCoordinates().GetY() << std::endl;
}
