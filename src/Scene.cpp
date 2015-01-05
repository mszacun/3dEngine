#include "Scene.h"
#include <cassert>

#define ZBUFFER_WIDTH 1024
#define ZBUFFER_HEIGHT 1024
void ClearZBuffer(double** zBuffer);
double infinity = std::numeric_limits<double>::max();

void Scene2D::AddTriangle(const Triangle2D& triangle)
{
    triangles_.push_back(triangle);
}

Scene3D::Scene3D() : worldTransformation_(4, 4)
{
    cam.position = Vector(0, 0, 0);
    cam.upDirection = Vector(0, 1, 0);
    cam.target = Vector(0, 0, 0);

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
    cam.position = newPosition;
}

void Scene3D::SetObservedPoint(const Vector& newObservedPoint)
{
    cam.target = newObservedPoint;
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
    Matrix transformationMatrix = worldTransformation_ * Matrix::CreateProjectMatrix(1);

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

double InterpolateZ(double betaz, double min, double max)
{
    return  1 / (betaz * (1 / min) + (1 - betaz) * (1 / max)); 
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
        double zl = InterpolateZ(betay, p1.GetZ(), p2.GetZ());

        double xr = betay * p1.GetX() + (1 - betay) * p3.GetX();
        double zr = InterpolateZ(betay, p1.GetZ(), p3.GetZ());

        for (int x = (int) xl; x < xr; x++)
        {
            double betax = CalculateDeltaY(x, xl, xr);
            double z = InterpolateZ(betax, zl, zr);
            if (zBuffer_[x][y] > z)
            {
                painter.setPen(shader.GetColorForPixel(Vector(x, y, z)));
                painter.drawPoint(x, y);
                zBuffer_[x][y] = z;
            }
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
    double zr = InterpolateZ(betay, top.GetZ(), bottom.GetZ());

    DrawTriangleWithXParellGround(top, Vector(xr, y, zr), middle, painter, shader);
    DrawTriangleWithXParellGround(bottom, Vector(xr, y, zr), middle, painter, shader);
}

void Scene3D::DrawProjectedTriangle(QPainter& painter, const Triangle3D& t, const Matrix& transformationMatrix)
{
    Triangle2D t2 = ProjectTrianglePerspectively(t, transformationMatrix);
    //PrintProjectInfo(t, t2);
    TriangleShadingInfo shadingInfo;

    shadingInfo.p1 = points_[t.GetP1()];
    shadingInfo.p2 = points_[t.GetP2()];
    shadingInfo.p3 = points_[t.GetP3()];

    shadingInfo.projectedP1 = t2.p1_;
    shadingInfo.projectedP2 = t2.p2_;
    shadingInfo.projectedP3 = t2.p3_;

    shadingInfo.p1Normal = pointsNormals_[t.GetP1()];
    shadingInfo.p2Normal = pointsNormals_[t.GetP2()];
    shadingInfo.p3Normal = pointsNormals_[t.GetP3()];

    shadingInfo.triangleNormal = t.GetNormal();

    shadingInfo.observatorPosition = cam.position;
    shadingInfo.lightPosition = lightPosition_;
    shadingInfo.lightColor = lightColor_;
    shadingInfo.ambientLightColor = ambientLightColor_;

    GouraudShader shader(shadingInfo);
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
}

QImage Scene3D::RenederPerspectiveProjection(int width, int height)
{
    QImage result(width, height, QImage::Format_ARGB32);
    QPainter painter(&result);

    painter.fillRect(0, 0, width, height, QColor("white"));
    ClearZBuffer(zBuffer_);

    Scene3D observedScene(*this);
    observedScene.ViewTransform();

    Matrix transformationMatrix = Matrix::CreateProjectMatrix(-cam.position.GetZ()) ;

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

    cam.Transform(transformationMatrix);
    lightPosition_ = lightPosition_.Transform(transformationMatrix);
}

Vector Scene3D::ProjectPoint(const Vector& p, const Matrix& projectionMatrix) const
{
    //Vector projected = p.Transform(projectionMatrix);
    //projected.SetZ(p.GetZ());

    Vector projected = Vector(p.GetX() / p.GetZ(), p.GetY() / p.GetZ(), p.GetZ());
    std::cout << p << " projected -> " << projected << std::endl;
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
    Transform(Matrix::CreateTranslationMatrix(-cam.target.GetX(),
        -cam.target.GetY(), -cam.target.GetZ()));

    // step 2
    double alfa = std::atan2(cam.position.GetX(), cam.position.GetZ());
    double fi = M_PI - alfa;
    Transform(Matrix::CreateYAxisRotationMatrix(fi));

    // step 3
    alfa = std::atan2(cam.position.GetZ(), cam.position.GetY());
    fi = -M_PI / 2 - alfa;
    Transform(Matrix::CreateXAxisRotationMatrix(fi));

    // step 4
    alfa = std::atan2(cam.upDirection.GetY(), cam.upDirection.GetX());
    fi = M_PI / 2 - alfa;
    Transform(Matrix::CreateZAxisRotationMatrix(fi));
    
    // step 5 - additional
    Transform(Matrix::CreateTranslationMatrix(-cam.position.GetX(),
        -cam.position.GetY(), -cam.position.GetZ()));

    double scaleXYFactor = 1 / (cam.zmax * std::tan(cam.viewAngle / 2));
    // step 6 - normalize coordinates
    Transform(Matrix::CreateScaleMatrix(scaleXYFactor, scaleXYFactor, 1 / cam.zmax));

    RecalculateNormals();
}

void ClearZBuffer(double** zBuffer)
{
    for (int i = 0; i < ZBUFFER_HEIGHT; i++)
        for (int j = 0; j < ZBUFFER_WIDTH; j++)
            zBuffer[i][j] = infinity;
}
