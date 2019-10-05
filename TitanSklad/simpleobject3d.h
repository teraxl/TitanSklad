#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include "iobjecttransform.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

struct VertexData
{
    VertexData() {}

    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p),
        textCoord(t),
        normal(n)
    {
    }

    QVector3D position;
    QVector2D textCoord;
    QVector3D normal;
};

class SimpleObject3D : public IObjectTransform
{
public:
    SimpleObject3D();
    SimpleObject3D(const QVector<VertexData> &vertData,
                   const QVector<GLuint> &index,
                   const QImage &texture);
    ~SimpleObject3D();

    void init(const QVector<VertexData> &vertData,
              const QVector<GLuint> &index,
              const QImage &texture);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture *m_texture;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    // IObjectTransform interface
public:
    virtual void rotate(const QQuaternion &r) override;
    virtual void translate(const QVector3D &t) override;
    virtual void scale(const float &s) override;
    virtual void setGlobalTransform(const QMatrix4x4 &g) override;
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
};

#endif // SIMPLEOBJECT3D_H
