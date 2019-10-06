#ifndef IOBJECTTRANSFORM_H
#define IOBJECTTRANSFORM_H

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector>

class QQuaternion;
class QMatrix4x4;
class QVector3D;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

class IObjectTransform
{
public:
    virtual void rotate(const QQuaternion &r) = 0;
    virtual void translate(const QVector3D &t) = 0;
    virtual void scale(const float &s) = 0;
    virtual void setGlobalTransform(const QMatrix4x4 &g) = 0;
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) = 0;
};

#endif // IOBJECTTRANSFORM_H
