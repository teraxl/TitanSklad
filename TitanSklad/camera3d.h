#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "iobjecttransform.h"
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Camera3D : public IObjectTransform
{
public:
    Camera3D();
    ~Camera3D();

    // IObjectTransform interface
public:
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0) override;

    void rotateX(const QQuaternion &rx);
    void rotateY(const QQuaternion &ry);

private:
    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QVector<IObjectTransform *> m_objects;
};

#endif // CAMERA3D_H
