#include <gtest/gtest.h>
#include "../src/ObjDeserializer.h"

template <typename T>
bool VectorsEqual(const std::vector<T>& left, const std::vector<T>& right)
{
    if (left.size() == right.size())
    {
        for (unsigned int i = 0; i < left.size(); i++)
        {
            if (! (left[i] == right[i]))
                return false;
        }
        return true;

    }
    else
        return false;
}

TEST(ObjDeserializerTests, ShouldParseVertexInfo)
{
    std::string vertexInfo = "v 0.981561 1.098386 0.836118";
    Vector expectedPoint(0.981561, 1.098386, 0.836118);
    ObjDeserializer deserializer;

    Vector deserialized = deserializer.ParseVertex(vertexInfo);


    EXPECT_TRUE(deserialized == expectedPoint);
}

TEST(ObjDeserializerTests, ShouldParseTriangleInfo)
{
    std::string triangleInfo = "f 1 2 3";
    Triangle3D expectedTriangle(1, 2, 3);
    ObjDeserializer deserializer;

    Triangle3D deserialized = deserializer.ParseTriangle(triangleInfo);


    // in obj file vertices are 1-base, we need them 0-based
    EXPECT_EQ(0, deserialized.GetP1());
    EXPECT_EQ(1, deserialized.GetP2());
    EXPECT_EQ(2, deserialized.GetP3());
}

TEST(ObjDeserializerTests, ShouldParseObjFile)
{
    std::string testObjFilePath = "tests/test.obj";
    ObjDeserializer deserializer;
    std::vector<Vector> expectedPoints 
    { 
        Vector(4.675601, 0.000000, 3.052211),
        Vector(-3.472256, 0.000000, 1.000000),
        Vector(1.000000, 0.000000, -3.762115),
        Vector(-1.000000, 0.000000, -3.074859)
    };
    std::vector<Triangle3D> expectedTriangles
    {
        Triangle3D(1, 0, 2),
        Triangle3D(1, 2, 3)
    };

    Scene3D deserializedScene = deserializer.ParseFile(testObjFilePath);

    EXPECT_TRUE(VectorsEqual(deserializedScene.GetPoints(), expectedPoints));
    EXPECT_TRUE(VectorsEqual(deserializedScene.GetTriangles(), expectedTriangles));
}
