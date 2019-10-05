#ifndef GROUP3D_H
#define GROUP3D_H
#include "iobjecttransform.h"
#include <QQuaternion>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

class Group3D : public IObjectTransform
{
private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QVector<IObjectTransform *> m_objects;

public:
    Group3D();

    // IObjectTransform interface
public:
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;

    void addObject(IObjectTransform *obj);
};

#endif // GROUP3D_H
