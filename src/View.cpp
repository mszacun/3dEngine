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

int RadToDeg(double rad)
{
    return (int) (rad * (180 / M_PI));
}

double DegToRad(int deg)
{
    return deg * (M_PI / 180);
}

void PerspectiveViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
}

ConfigurationPanel::ConfigurationPanel(View* view) : xCameraPostitionLabel_("X: "),
    yCameraPostitionLabel_("Y: "), zCameraPostitionLabel_("Z: "),
    cameraViewAngleControls_("View angle: "), cameraViewAngleSlider_(Qt::Horizontal),
    shadowsTypeRadioButtons_("Shader alghorithm: "), flatShadingRadioButton_("Flat Shading"),
    gouroudShadingRadioButton_("Gouraud shading"), phongShadingRadioButton_("Phong shading"),
    openFileButton_("Open file"), saveFileButton_("Save file"), view_(view)
{
    coordinatesValidator_.setRange(-20, 20, 10);
    cameraViewAngleSlider_.setRange(1, 89);

    xCameraPostitionEdit_.setValidator(&coordinatesValidator_);
    yCameraPostitionEdit_.setValidator(&coordinatesValidator_);
    zCameraPostitionEdit_.setValidator(&coordinatesValidator_);

    shadowsTypeRadioButtonsLayout_.addWidget(&flatShadingRadioButton_);
    shadowsTypeRadioButtonsLayout_.addWidget(&gouroudShadingRadioButton_);
    shadowsTypeRadioButtonsLayout_.addWidget(&phongShadingRadioButton_);
    shadowsTypeRadioButtons_.setLayout(&shadowsTypeRadioButtonsLayout_);
    phongShadingRadioButton_.setChecked(true);

    cameraViewAngleControlsLayout_.addWidget(&cameraViewAngleLabel_);
    cameraViewAngleControlsLayout_.addWidget(&cameraViewAngleSlider_);
    cameraViewAngleControls_.setLayout(&cameraViewAngleControlsLayout_);

    mainLayout_.addWidget(&xCameraPostitionLabel_, 0, 0);
    mainLayout_.addWidget(&xCameraPostitionEdit_, 1 ,0);
    mainLayout_.addWidget(&yCameraPostitionLabel_, 2, 0);
    mainLayout_.addWidget(&yCameraPostitionEdit_, 3, 0);
    mainLayout_.addWidget(&zCameraPostitionLabel_, 4, 0);
    mainLayout_.addWidget(&zCameraPostitionEdit_, 5, 0);
    mainLayout_.addWidget(&cameraViewAngleControls_, 6, 0);
    mainLayout_.addWidget(&shadowsTypeRadioButtons_, 7, 0);
    mainLayout_.addWidget(&openFileButton_, 8, 0);
    mainLayout_.addWidget(&saveFileButton_, 9, 0);

    QObject::connect(&xCameraPostitionEdit_, SIGNAL(returnPressed()),
            this, SLOT(OnXCameraPositionEntered()));
    QObject::connect(&yCameraPostitionEdit_, SIGNAL(returnPressed()),
            this, SLOT(OnYCameraPositionEntered()));
    QObject::connect(&zCameraPostitionEdit_, SIGNAL(returnPressed()),
            this, SLOT(OnZCameraPositionEntered()));
    QObject::connect(&cameraViewAngleSlider_, SIGNAL(sliderMoved(int)),
            this, SLOT(OnViewAngleSliderMoved(int)));

    QObject::connect(&flatShadingRadioButton_, SIGNAL(toggled(bool)),
            this, SLOT(OnShadowingTypeChanged(bool)));
    QObject::connect(&gouroudShadingRadioButton_, SIGNAL(toggled(bool)),
            this, SLOT(OnShadowingTypeChanged(bool)));
    QObject::connect(&phongShadingRadioButton_, SIGNAL(toggled(bool)),
            this, SLOT(OnShadowingTypeChanged(bool)));

    setLayout(&mainLayout_);
}

void ConfigurationPanel::UpdateCameraParameters(const PerspectiveCamera& camera)
{
    xCameraPostitionEdit_.setText(std::to_string(camera.position.GetX()).c_str());
    yCameraPostitionEdit_.setText(std::to_string(camera.position.GetY()).c_str());
    zCameraPostitionEdit_.setText(std::to_string(camera.position.GetZ()).c_str());

    cameraViewAngleLabel_.setText(std::to_string(RadToDeg(camera.GetViewAngle())).c_str());
    cameraViewAngleSlider_.setValue(RadToDeg(camera.GetViewAngle()));
}

void ConfigurationPanel::OnXCameraPositionEntered()
{
    controler_->SetCameraXCoordinate(std::stod(xCameraPostitionEdit_.text().toLocal8Bit().data()));
}

void ConfigurationPanel::OnYCameraPositionEntered()
{
    controler_->SetCameraYCoordinate(std::stod(yCameraPostitionEdit_.text().toLocal8Bit().data()));
}

void ConfigurationPanel::OnZCameraPositionEntered()
{
    controler_->SetCameraZCoordinate(std::stod(zCameraPostitionEdit_.text().toLocal8Bit().data()));
}

void ConfigurationPanel::OnViewAngleSliderMoved(int value)
{
    controler_->SetCameraViewAngle(DegToRad(value));
    view_->UpdateCameraViews();
}

void ConfigurationPanel::OnShadowingTypeChanged(bool checked)
{
    if (flatShadingRadioButton_.isChecked())
    {
        controler_->SetFlatShader();
        std::cout << "Flat" << std::endl;
    }
    if (gouroudShadingRadioButton_.isChecked())
    {
        controler_->SetGouroudShader();
        std::cout << "Gouroud" << std::endl;
    }
    if (phongShadingRadioButton_.isChecked())
    {
        controler_->SetPhongShader();
        std::cout << "Phong" << std::endl;
    }
    view_->UpdateCameraViews();
}

View::View() : frontView_(this), sideView_(this), topView_(this),
    configurationPanel_(this)
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
    return QSize(1300, 1000);
}

QSize View::sizeHint() const
{
    return minimumSizeHint();
}

void View::SetControler(ControlerPtr controler)
{
    controler_ = controler;
    configurationPanel_.SetControler(controler);
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
