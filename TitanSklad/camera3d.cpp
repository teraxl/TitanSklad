#include "camera3d.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

Camera3D::Camera3D()
{
    m_scale = 1.0f;
    m_globalTransform.setToIdentity();
}

Camera3D::~Camera3D()
{

}

void Camera3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void Camera3D::translate(const QVector3D &t)
{
    m_translate += t;
}

void Camera3D::scale(const float &s)
{
    m_scale *= s;
}

void Camera3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
}

void Camera3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (functions != nullptr) return;

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(m_translate);
    viewMatrix.rotate(m_rotate);
    viewMatrix.scale(m_scale);
    viewMatrix = m_globalTransform * viewMatrix;

    program->setUniformValue("u_viewMatrix", viewMatrix);
}

void Camera3D::rotateX(const QQuaternion &rx)
{
    m_rotateX = rx * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;
}

void Camera3D::rotateY(const QQuaternion &ry)
{
    m_rotateY = ry * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;
}
