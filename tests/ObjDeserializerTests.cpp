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
    Point expectedPoint(0.981561, 1.098386, 0.836118);
    ObjDeserializer deserializer;

    Point deserialized = deserializer.ParseVertex(vertexInfo);


    EXPECT_TRUE(deserialized == expectedPoint);
}

TEST(ObjDeserializerTests, ShouldParseTriangleInfo)
{
    std::string triangleInfo = "f 1 2 3";
    Triangle3D expectedTriangle(1, 2, 3);
    ObjDeserializer deserializer;

    Triangle3D deserialized = deserializer.ParseTriangle(triangleInfo);


    EXPECT_EQ(1, deserialized.GetP1());
    EXPECT_EQ(2, deserialized.GetP2());
    EXPECT_EQ(3, deserialized.GetP3());
}

TEST(ObjDeserializerTests, ShouldParseObjFile)
{
    std::string testObjFilePath = "tests/test.obj";
    ObjDeserializer deserializer;
    std::vector<Point> expectedPoints 
    { 
        Point(4.675601, 0.000000, 3.052211),
        Point(-3.472256, 0.000000, 1.000000),
        Point(1.000000, 0.000000, -3.762115),
        Point(-1.000000, 0.000000, -3.074859)
    };
    std::vector<Triangle3D> expectedTriangles
    {
        Triangle3D(2, 1, 3),
        Triangle3D(2, 3, 4)
    };

    Scene3D deserializedScene = deserializer.ParseFile(testObjFilePath);

    EXPECT_TRUE(VectorsEqual(deserializedScene.GetPoints(), expectedPoints));
    EXPECT_TRUE(VectorsEqual(deserializedScene.GetTriangles(), expectedTriangles));
}
