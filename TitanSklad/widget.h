#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

class SimpleObject3D;
class IObjectTransform;
class Group3D;
class Camera3D;

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    void initShaders();
    void initCube(float width);
    void initPlane(float size);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_programs;

    GLfloat m_xRotate;
    GLfloat m_yRotate;
    QPoint m_ptPosition;

    QVector2D m_mousePosition;
    //QQuaternion m_rotation;

    QVector<SimpleObject3D *> m_objects;
    QVector<Group3D *> m_groups;
    QVector<IObjectTransform *> m_TransformObjects;

    //float m_z;

    Camera3D *m_camera;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // WIDGET_H
