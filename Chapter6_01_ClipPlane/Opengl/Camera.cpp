#include "Camera.h"

Camera::Camera() {}
// constructor with vectors
Camera::Camera(QVector3D position)
    :MovementSpeed( 2.5f),
      MouseSensitivity( 0.1f),
      Zoom(45.0f)
{
    Position = position;
    WorldUp = QVector3D(0.0f, 1.0f, 0.0f);
    updateCameraVectors();
}
QMatrix4x4 Camera::GetViewMatrix() {
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.lookAt(Position, _target, Up);
    return  mat;
}
// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        Position -= Front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        Position += Front * velocity;
    if (direction == Camera_Movement::LEFT)
        Position += Right * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position -= Right * velocity;
    updateCameraVectors();
}
// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch ) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    qDebug() << xoffset << "-" << yoffset;
    Yaw += yoffset;
    Pitch += xoffset;
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    // if (constrainPitch)
    // {
    /*        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;*/
    // }
    double x = cos(DegToRadian(Pitch));
    double y = sin(DegToRadian(Yaw));
    double z = sin(DegToRadian(Pitch));
    QVector3D newPoint(x, y, z);
    newPoint.normalize();
    auto length = Position.distanceToPoint(_target);
    Position = newPoint * length;
    updateCameraVectors();
}
// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
    Position = QVector3D(Position.x() - Front.x() * yoffset, Position.y() - Front.y() * yoffset, Position.z() - Front.z() * yoffset);
}

void Camera::updateCameraVectors() {
    Front = QVector3D(Position.x() - _target.x(), Position.y() - _target.y(), Position.z() - _target.z());
    Front.normalize();
    Right = QVector3D::crossProduct(Front, WorldUp);
    Right.normalize();
    Up = QVector3D::crossProduct(Right, Front);
    Up.normalize();
}

float Camera::DegToRadian(float degree) {
    return  degree * M_PI / 180;
}
