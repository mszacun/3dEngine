#include "Camera.h"

void Camera::Transform(const Matrix& transformationMatrix)
{
    position = position.Transform(transformationMatrix);
    upDirection = upDirection.Transform(transformationMatrix);
    target = target.Transform(transformationMatrix);
}

PerspectiveCameraFrustrum PerspectiveCamera::CalculateFrustrum() const
{
    Vector viewDirectionVector = (target - position).Normalize();
    Vector u = upDirection.Cross(viewDirectionVector).Normalize();
    Vector A = position + viewDirectionVector * zmax;
    Vector B = A + u *  zmax * std::tan(viewAngle_ / 2);
    Vector C = A + upDirection *  zmax * std::tan(viewAngle_ / 2);

    Vector leftTop = C - u *  zmax * std::tan(viewAngle_ / 2);
    Vector rightBottom = B - upDirection *  zmax * std::tan(viewAngle_ / 2);

    return { leftTop, rightBottom, position, target };
}
