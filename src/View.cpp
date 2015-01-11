#include "View.h"
#include "Controler.h"
#include <chrono>

void OrthagonalViewport::DrawScene(QImage& scene, const Vector& perspectiveCameraPosition)
{
    buffer_ = scene;
    int width = 500;
    int height = 500;
    int cameraX = (perspectiveCameraPosition.GetX() * (width / 2) + width / 2);
    int cameraY = (perspectiveCameraPosition.GetY() * (height / 2) + height / 2);
    cameraRect_ = QRect(cameraX - 5, cameraY - 5, 10, 10);
    update();
}

void OrthagonalViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
    painter.fillRect(cameraRect_, QColor("lime"));
}

void OrthagonalViewport::mousePressEvent(QMouseEvent* event)
{
    cameraMovingFlag_ = cameraRect_.contains(event->x(), event->y());
    lastMousePosition_ = event->pos();
}

void OrthagonalViewport::mouseReleaseEvent(QMouseEvent* event)
{
    cameraMovingFlag_ = false;
}

void OrthagonalViewport::mouseMoveEvent(QMouseEvent* event)
{
    if (cameraMovingFlag_)
    {
        int deltaX = event->x() - lastMousePosition_.x();
        int deltaY = event->y() - lastMousePosition_.y();
        lastMousePosition_ = event->pos();
        Vector cameraMoveVector = GetCameraTranslation(deltaX, deltaY);
        cameraMoveVector = cameraMoveVector * 0.01875;

        view_->MoveCamera(cameraMoveVector);
    }
}

void PerspectiveViewport::DrawScene(QImage& scene)
{
    buffer_ = scene;
    update();
}

void PerspectiveViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
}

ConfigurationPanel::ConfigurationPanel() : xCameraPostitionLabel_("X: "),
    yCameraPostitionLabel_("Y: "), zCameraPostitionLabel_("Z: "),
    cameraViewAngleLabel_("View angle: "), cameraViewAngleSlider_(Qt::Horizontal),
    shadowsTypeRadioButtons_("Shader alghorithm: "), flatShadingRadioButton_("Flat Shading"),
    gouroudShadingRadioButton_("Gouraud shading"), phongShadingRadioButton_("Phong shading"),
    openFileButton_("Open file"),
    saveFileButton_("Save file")
{
    shadowsTypeRadioButtonsLayout_.addWidget(&flatShadingRadioButton_);
    shadowsTypeRadioButtonsLayout_.addWidget(&gouroudShadingRadioButton_);
    shadowsTypeRadioButtonsLayout_.addWidget(&phongShadingRadioButton_);
    shadowsTypeRadioButtons_.setLayout(&shadowsTypeRadioButtonsLayout_);

    mainLayout_.addWidget(&xCameraPostitionLabel_, 0, 0);
    mainLayout_.addWidget(&xCameraPostitionEdit_, 1 ,0);
    mainLayout_.addWidget(&yCameraPostitionLabel_, 2, 0);
    mainLayout_.addWidget(&yCameraPostitionEdit_, 3, 0);
    mainLayout_.addWidget(&zCameraPostitionLabel_, 4, 0);
    mainLayout_.addWidget(&zCameraPostitionEdit_, 5, 0);
    mainLayout_.addWidget(&cameraViewAngleLabel_, 6, 0);
    mainLayout_.addWidget(&cameraViewAngleSlider_, 7, 0);
    mainLayout_.addWidget(&shadowsTypeRadioButtons_, 8, 0);
    mainLayout_.addWidget(&openFileButton_, 9, 0);
    mainLayout_.addWidget(&saveFileButton_, 10, 0);

    setLayout(&mainLayout_);
}

void ConfigurationPanel::UpdateCameraParameters(const PerspectiveCamera& camera)
{
    xCameraPostitionEdit_.setText(std::to_string(camera.position.GetX()).c_str());
    yCameraPostitionEdit_.setText(std::to_string(camera.position.GetY()).c_str());
    zCameraPostitionEdit_.setText(std::to_string(camera.position.GetZ()).c_str());
}

View::View() : frontView_(this), sideView_(this), topView_(this)
{
    setFocusPolicy(Qt::ClickFocus);
    
    layout.addWidget(&perspectiveView_, 0, 0);
    layout.addWidget(&frontView_, 0, 1);
    layout.addWidget(&sideView_, 1, 1);
    layout.addWidget(&topView_, 1, 0);
    layout.addWidget(&configurationPanel_, 0, 2, 2, 1);

    layout.setColumnMinimumWidth(0, 500);
    layout.setColumnMinimumWidth(1, 500);

    setLayout(&layout);
}

void View::MoveCamera(const Vector& moveVector)
{
    controler_->MoveCamera(moveVector);
    UpdateCameraViews();
}

QSize View::minimumSizeHint() const
{
    return QSize(1000, 1000);
}

QSize View::sizeHint() const
{
    return minimumSizeHint();
}

void View::SetControler(ControlerPtr controler)
{
    controler_ = controler;
    UpdateCameraViews();
}

void View::UpdateCameraViews()
{
    auto start = std::chrono::steady_clock::now();

    QImage i = controler_->GetRenderedPerspectiveView();
    perspectiveView_.DrawScene(i);

    OrthogonalProjection projection = controler_->GetFrontView();
    frontView_.DrawScene(projection.renderedImage, projection.perspectiveCameraPosition);

    projection = controler_->GetSideView();
    sideView_.DrawScene(projection.renderedImage, projection.perspectiveCameraPosition);

    projection = controler_->GetTopView();
    topView_.DrawScene(projection.renderedImage, projection.perspectiveCameraPosition);

    configurationPanel_.UpdateCameraParameters(controler_->GetPerspectiveCamera());

    auto end = std::chrono::steady_clock::now();


    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Drawing frame: " << dur << std::endl;
    std::cout << "FPS: " << 1000.0 / dur << std::endl;
}

void View::keyPressEvent(QKeyEvent* event)
{
    controler_->KeyPressed(event->key());
    UpdateCameraViews();
    QWidget::keyPressEvent(event);
}
