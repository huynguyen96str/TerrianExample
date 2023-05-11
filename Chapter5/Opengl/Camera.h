#ifndef CAMERA_H
#define CAMERA_H


#include <QVector3D>
#include <QMatrix4x4>
#include <qdebug.h>
#include "Opengl/GlobalOpengl.h"

class Camera
{
public:
    // camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;

    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    Camera();
    // constructor with vectors
    Camera(QVector3D position);
    QMatrix4x4 GetViewMatrix();
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
private:
    QVector3D WorldUp;
    QVector3D _target;
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    float DegToRadian(float degree);
};

#endif // CAMERA_H
