#include "View.h"
#include "Controler.h"
#include <chrono>

void OrthagonalViewport::DrawScene(QImage& scene,
        const PerspectiveCameraFrustrum& frustrum)
{
    buffer_ = scene;
    int width = VIEWPORT_WIDTH;
    int height = VIEWPORT_HEIGHT;
    int cameraX = (frustrum.cameraPosition.GetX() * (width / 2) + width / 2);
    int cameraY = (frustrum.cameraPosition.GetY() * (height / 2) + height / 2);
    int targetPointX = (frustrum.target.GetX() * (width / 2) + width / 2);
    int targetPointY = (frustrum.target.GetY() * (height / 2) + height / 2);
    cameraRect_ = QRect(cameraX - 5, cameraY - 5, 10, 10);
    targetPointRect_ = QRect(targetPointX - 5, targetPointY - 5, 10, 10);
    frustrum_ = frustrum;
    update();
}

void OrthagonalViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
    DrawPerspectiveCameraFrustrum(painter, frustrum_);
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(targetPointRect_);
    painter.fillRect(cameraRect_, QColor("lime"));
}

void OrthagonalViewport::mousePressEvent(QMouseEvent* event)
{
    cameraMovingFlag_ = cameraRect_.contains(event->x(), event->y());
    targetPointMovingFlag_ = targetPointRect_.contains(event->x(), event->y());
    lastMousePosition_ = event->pos();
}

void OrthagonalViewport::mouseReleaseEvent(QMouseEvent* event)
{
    cameraMovingFlag_ = false;
    targetPointMovingFlag_ = false;
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
    if (targetPointMovingFlag_)
    {
        int deltaX = event->x() - lastMousePosition_.x();
        int deltaY = event->y() - lastMousePosition_.y();
        lastMousePosition_ = event->pos();
        Vector cameraMoveVector = GetCameraTranslation(deltaX, deltaY);
        cameraMoveVector = cameraMoveVector * 0.01875;

        view_->MoveTargetPoint(cameraMoveVector);
    }
}

void OrthagonalViewport::DrawPerspectiveCameraFrustrum(QPainter& painter,
        const PerspectiveCameraFrustrum& frustrum)
{
    int width = VIEWPORT_WIDTH;
    int height = VIEWPORT_HEIGHT;

    int cameraX = (frustrum_.cameraPosition.GetX() * (width / 2) + width / 2);
    int cameraY = (frustrum_.cameraPosition.GetY() * (height / 2) + height / 2);

    int leftUpperX = (frustrum_.leftTop.GetX() * (width / 2) + width / 2);
    int leftUpperY = (frustrum_.leftTop.GetY() * (height / 2) + height / 2);
    int rightBottomX = (frustrum_.rightBottom.GetX() * (width / 2) + width / 2);
    int rightBottomY = (frustrum_.rightBottom.GetY() * (height / 2) + height / 2);

    QPen penHLines(QColor("#0e5a77"));
    painter.setPen(penHLines);
    painter.drawLine(leftUpperX, leftUpperY, rightBottomX, leftUpperY);
    painter.drawLine(rightBottomX, leftUpperY, rightBottomX, rightBottomY);
    painter.drawLine(rightBottomX, rightBottomY, leftUpperX, rightBottomY);
    painter.drawLine(leftUpperX, rightBottomY, leftUpperX, leftUpperY);

    painter.drawLine(leftUpperX, leftUpperY, cameraX, cameraY);
    painter.drawLine(rightBottomX, leftUpperY, cameraX, cameraY);
    painter.drawLine(rightBottomX, rightBottomY, cameraX, cameraY);
    painter.drawLine(leftUpperX, rightBottomY, cameraX, cameraY);
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

    layout.setColumnMinimumWidth(0, VIEWPORT_WIDTH);
    layout.setColumnMinimumWidth(1, VIEWPORT_HEIGHT);

    setLayout(&layout);
}

void View::MoveCamera(const Vector& moveVector)
{
    controler_->MoveCamera(moveVector);
    UpdateCameraViews();
}

void View::MoveTargetPoint(const Vector& moveVector)
{
    controler_->MoveTargetPoint(moveVector);
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
    frontView_.DrawScene(projection.renderedImage, projection.perspectiveCameraFrustrum);

    projection = controler_->GetSideView();
    sideView_.DrawScene(projection.renderedImage, projection.perspectiveCameraFrustrum);

    projection = controler_->GetTopView();
    topView_.DrawScene(projection.renderedImage, projection.perspectiveCameraFrustrum);

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
