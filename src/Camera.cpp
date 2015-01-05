#include "Camera.h"

void Camera::Transform(const Matrix& transformationMatrix)
{
    position = position.Transform(transformationMatrix);
    upDirection = upDirection.Transform(transformationMatrix);
    target = target.Transform(transformationMatrix);
}
