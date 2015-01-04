#include "Scene.h"
#include <cassert>

#define ZBUFFER_WIDTH 1024
#define ZBUFFER_HEIGHT 1024
void ClearZBuffer(double** zBuffer);

void Scene2D::AddTriangle(const Triangle2D& triangle)
{
    triangles_.push_back(triangle);
}

Scene3D::Scene3D() : observatorPosition_(0, 0, 0), observedPoint_(0, 0, 0),
    upDirection_(0, 1, 0), worldTransformation_(4, 4)
{
    zBuffer_ = new double*[ZBUFFER_HEIGHT];

    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
        zBuffer_[i] = new double[ZBUFFER_WIDTH];

    worldTransformation_ = Matrix::CreateIdentityMatrix(4);
}

Scene3D::~Scene3D()
{
//    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
//        delete[] zBuffer_[i];

//    delete[] zBuffer_;
}

void Scene3D::AddPoint(const int& x, const int& y, const int& z)
{
    points_.push_back(Vector(x, y, z));
}

void Scene3D::AddPoint(const Vector& p)
{
    points_.push_back(p);
}

void Scene3D::AddTriangle(const int& p1, const int& p2, const int& p3)
{
    Triangle3D triangle(p1, p2, p3);
    AddTriangle(triangle);
}

void Scene3D::AddTriangle(Triangle3D& triangle)
{
    triangles_.push_back(triangle);
}

Vector Scene3D::CalculateNormal(const Triangle3D& triangle) const
{
    Vector v1 = points_[triangle.GetP2()] - points_[triangle.GetP1()];
    Vector v2 = points_[triangle.GetP3()] - points_[triangle.GetP2()];
    Vector normal = v1.Cross(v2).Normalize();

    std::cout << "Calculated normal: " << normal << std::endl;
    return normal;
}

Vector Scene3D::CalculatePointNormal(unsigned int pointNumber) const
{
    Vector sum(0, 0, 0);

    for (const Triangle3D& t : triangles_)
    {
        if (t.GetP1() == pointNumber || t.GetP2() == pointNumber ||
                t.GetP3() == pointNumber)
            sum = sum + t.GetNormal();
    }

    return sum.Normalize();
}

void Scene3D::RecalculateNormals()
{
    for (Triangle3D& t : triangles_)
        t.SetNormal(CalculateNormal(t));

    pointsNormals_.clear();
    for (unsigned int i = 0; i < points_.size(); i++)
        pointsNormals_.push_back(CalculatePointNormal(i));
}

void Scene3D::SetObserverPosition(const Vector& newPosition)
{
    observatorPosition_ = newPosition;
}

void Scene3D::SetObservedPoint(const Vector& newObservedPoint)
{
    observedPoint_ = newObservedPoint;
}

void Scene3D::SetLightPosition(const Vector& newPosition)
{
    lightPosition_ = newPosition;
}

void Scene3D::SetLightColor(const QColor& color)
{
    lightColor_ = color;
}

Scene2D Scene3D::GetPerspectiveProjection() const
{
    Scene2D result;
    Matrix transformationMatrix = worldTransformation_ * Matrix::CreateProjectMatrix(-observatorPosition_.GetZ());

    for (const Triangle3D& t : triangles_)
        result.AddTriangle(ProjectTrianglePerspectively(t, transformationMatrix));

    return result;
}

void SortVertices(Vector& top, Vector& middle, Vector& bottom)
{
    if (top.GetY() > middle.GetY())
        std::swap(top, middle);

    if (middle.GetY() > bottom.GetY())
        std::swap(middle, bottom);

    if (top.GetY() > middle.GetY())
        std::swap(top, middle);
}

double CalculateDeltaY(double y, double ystart, double yend)
{
    return (ystart == yend) ? 1 : (y - yend) / (ystart - yend);
}

void Scene3D::DrawTriangleWithXParellGround(const Vector& p1, Vector p2, Vector p3, QPainter& painter, Shader& shader)
{
    // inv: p2 and p3 are on the same line, p1 is peak of triangle
    assert(p2.GetY() == p3.GetY());
    // we want to draw from left to right
    if (p2.GetX() > p3.GetX())
        std::swap(p2, p3);

    // in case of problems try changing to double
    int ystart = (int) p1.GetY();
    int yend = (int) p2.GetY();
    int direction = ystart < yend ? 1 : -1;
    for (int y = ystart; y != yend + direction; y += direction)
    {
        double betay = CalculateDeltaY(y, ystart, yend);
        double xl = betay * p1.GetX() + (1 - betay) * p2.GetX();
        double xr = betay * p1.GetX() + (1 - betay) * p3.GetX();

        for (int x = (int) xl; x < xr; x++)
        {
            painter.setPen(shader.GetColorForPixel(Vector(x, y, 0)));
            painter.drawPoint(x, y);
        }

    }
}

void Scene3D::DrawTriangle(const Vector& p1, const Vector& p2, const Vector& p3, QPainter& painter, Shader& shader)
{
    Vector top = p1, middle = p2, bottom = p3;
    SortVertices(top, middle, bottom);

    // inv: p1.y <= p2.y <= p3.y
    double y = middle.GetY();
    double betay = CalculateDeltaY(y, top.GetY(), bottom.GetY());
    double xr = betay * top.GetX() + (1 - betay) * bottom.GetX();

    DrawTriangleWithXParellGround(top, Vector(xr, y, 0), middle, painter, shader);
    DrawTriangleWithXParellGround(bottom, Vector(xr, y, 0), middle, painter, shader);
}

void Scene3D::DrawProjectedTriangle(QPainter& painter, const Triangle3D& t, const Matrix& transformationMatrix)
{
    Triangle2D t2 = ProjectTrianglePerspectively(t, transformationMatrix);
    PrintProjectInfo(t, t2);
    TriangleShadingInfo shadingInfo;

    shadingInfo.p1 = points_[t.GetP1()];
    shadingInfo.p2 = points_[t.GetP2()];
    shadingInfo.p3 = points_[t.GetP3()];

    shadingInfo.p1Normal = pointsNormals_[t.GetP1()];
    shadingInfo.p2Normal = pointsNormals_[t.GetP2()];
    shadingInfo.p3Normal = pointsNormals_[t.GetP3()];

    shadingInfo.triangleNormal = t.GetNormal();

    shadingInfo.observatorPosition = observatorPosition_;
    shadingInfo.lightPosition = lightPosition_;
    shadingInfo.lightColor = lightColor_;

    FlatShader shader(shadingInfo);
    DrawTriangle(t2.p1_, t2.p2_, t2.p3_, painter, shader);
}

void Scene3D::PrintProjectInfo(const Triangle3D& t, const Triangle2D& t2) const
{
    std::cout << "Projectiing trinagle" << std::endl;
    std::cout << points_[t.GetP1()] << " -> " << t2.p1_ << std::endl;
    std::cout << points_[t.GetP2()] << " -> " << t2.p2_ << std::endl;
    std::cout << points_[t.GetP3()] << " -> " << t2.p3_ << std::endl;
}

void Scene3D::DrawScene(QPainter& painter, const Matrix& transformationMatrix)
{
    for (const Triangle3D& t : triangles_)
        DrawProjectedTriangle(painter, t, transformationMatrix);
    // {
    //     painter.drawLine((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), (int) t2.p2_.GetX(), (int) t2.p2_.GetY());
    //     painter.drawLine((int) t2.p2_.GetX(), (int) t2.p2_.GetY(), (int) t2.p3_.GetX(), (int) t2.p3_.GetY());
    //     painter.drawLine((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), (int) t2.p3_.GetX(), (int) t2.p3_.GetY());
    //     painter.drawEllipse((int) t2.p1_.GetX(), (int) t2.p1_.GetY(), 1, 1);
    //}
}

QImage Scene3D::RenederPerspectiveProjection(int width, int height)
{
    QImage result(width, height, QImage::Format_ARGB32);
    QPainter painter(&result);

    painter.fillRect(0, 0, width, height, QColor("white"));
    ClearZBuffer(zBuffer_);

    Scene3D observedScene(*this);
    observedScene.ViewTransform();

    Matrix transformationMatrix = Matrix::CreateProjectMatrix(-observedScene.observatorPosition_.GetZ()) ;

    std::cout << "Transformation matrix: " << std::endl;
    transformationMatrix.Print();

    observedScene.DrawScene(painter, transformationMatrix);

    return result;
}

void Scene3D::AccumulateTransformation(const Matrix& transformationMatrix)
{
    worldTransformation_ = transformationMatrix * worldTransformation_;
}

void Scene3D::Transform(const Matrix& transformationMatrix)
{
    for (Vector& p : points_)
        p = p.Transform(transformationMatrix);

    observatorPosition_ = observatorPosition_.Transform(transformationMatrix);
    lightPosition_ = lightPosition_.Transform(transformationMatrix);
    observedPoint_ = observedPoint_.Transform(transformationMatrix);
    upDirection_ = upDirection_.Transform(transformationMatrix);
}

Vector Scene3D::ProjectPoint(const Vector& p, const Matrix& projectionMatrix) const
{
    Vector projected = p.Transform(projectionMatrix);
    projected.SetZ(p.GetZ());

    return projected;
}

Triangle2D Scene3D::ProjectTrianglePerspectively(const Triangle3D& triangle,
        const Matrix& transformationMatrix) const
{
     return Triangle2D(ProjectPoint(points_[triangle.GetP1()], transformationMatrix),
         ProjectPoint(points_[triangle.GetP2()], transformationMatrix),
         ProjectPoint(points_[triangle.GetP3()], transformationMatrix));
}

void Scene3D::ViewTransform()
{
    /* step 1 */
    Transform(Matrix::CreateTranslationMatrix(-observedPoint_.GetX(),
                -observedPoint_.GetY(), -observedPoint_.GetZ()));

    // step 2
    double alfa = std::atan2(observatorPosition_.GetX(), observatorPosition_.GetZ());
    double fi = M_PI - alfa;
    Transform(Matrix::CreateYAxisRotationMatrix(fi));

    // step 3
    alfa = std::atan2(observatorPosition_.GetZ(), observatorPosition_.GetY());
    fi = -M_PI / 2 - alfa;
    Transform(Matrix::CreateXAxisRotationMatrix(fi));

    // step 4
    alfa = std::atan2(upDirection_.GetY(), upDirection_.GetX());
    fi = M_PI / 2 - alfa;
    Transform(Matrix::CreateZAxisRotationMatrix(fi));

    RecalculateNormals();

    std::cout << "Observer position after transformation: " << observatorPosition_ << std::endl;

}

void ClearZBuffer(double** zBuffer)
{
    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
        for (int j = 0; j < ZBUFFER_WIDTH; j++)
            zBuffer[i][j] = -666;
}
