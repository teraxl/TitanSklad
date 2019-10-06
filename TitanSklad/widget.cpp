#include "camera3d.h"
#include "widget.h"

#include <QDebug>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_5_Core>
#include <QImage>
#include <QOpenGLTexture>
#include "simpleobject3d.h"
#include "group3d.h"
#include <QOpenGLContext>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    //m_z = -5.0f;
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

Widget::~Widget()
{
    delete m_camera;

    for (int i = 0; i < m_objects.size(); ++i) {
        delete m_objects[i];
    }
    for (int i = 0; i < m_groups.size(); ++i) {
        delete m_groups[i];
    }
    for (int i = 0; i < m_TransformObjects.size(); ++i) {
        delete m_TransformObjects[i];
    }
}

void Widget::initializeGL()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    float step = 2.0f;

    m_groups.append(new Group3D);
    initCube(2.0f);
    m_objects[0]->translate(QVector3D(-8.0f, 0.0f, -2.0f));
    m_groups[0]->addObject(m_objects[0]);

    initPlane(1.0f);
    m_objects[1]->translate(QVector3D(0.0f, 0.5f, 0.0f));
    m_groups[0]->addObject(m_objects[1]);

    m_groups[0]->translate(QVector3D(0.0f, 0.0f, 0.0f));
    m_TransformObjects.append(m_groups[0]);

}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / static_cast<float>(h);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.01f, 10000.0f);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    QMatrix4x4 viewMatrix;
//    viewMatrix.setToIdentity();
//    viewMatrix.translate(0.0f, 0.0f, m_z);
//    viewMatrix.rotate(m_rotation);
//    viewMatrix.rotate(QQuaternion(0.872666f, 0.164907f, -0.452986f, -0.0778399f));

    m_programs.bind();
    m_programs.setUniformValue("u_projectionMatrix", m_projectionMatrix);
//    m_programs.setUniformValue("u_viewMatrix", viewMatrix);
    m_programs.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_programs.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&m_programs);

    for (int i = 0; i < m_TransformObjects.size(); ++i) {
        m_TransformObjects[i]->draw(&m_programs, context()->functions());
    }
}

void Widget::initShaders()
{
    if (!m_programs.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh")) {
        close();
    }

    if (!m_programs.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh")) {
        close();
    }

    if (!m_programs.link()) {
        close();
    }
}

void Widget::initCube(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  width_div_2),  QVector2D(0.0f, 1.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  width_div_2),  QVector2D(0.0f, 0.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, width_div_2),  QVector2D(1.0f, 1.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),  QVector2D(1.0f, 0.0f), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  width_div_2),  QVector2D(0.0f, 1.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, width_div_2),  QVector2D(0.0f, 0.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  width_div_2),  QVector2D(0.0f, 1.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  width_div_2),  QVector2D(1.0f, 1.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  -width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  width_div_2,  -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  width_div_2),  QVector2D(0.0f, 1.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2,  -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),  QVector2D(1.0f, 1.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),  QVector2D(0.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, width_div_2),  QVector2D(1.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,  -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;

    for (GLuint i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage(":/texture/yachik2.jpg")));
}

void Widget::initPlane(float size)
{
    float sizePlane = 2.0f; //size * 1.0f;
    float m_x = 22.0f;
    float m_y = 1.5f;
    float m_z = 58.0f;
    QVector<VertexData> vertex;
//    vertex.append(VertexData(QVector3D(-sizePlane, -sizePlane, -sizePlane),  QVector2D(0.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
//    vertex.append(VertexData(QVector3D(sizePlane, -sizePlane, -sizePlane), QVector2D(0.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
//    vertex.append(VertexData(QVector3D(-sizePlane,  sizePlane, -sizePlane),  QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
//    vertex.append(VertexData(QVector3D(sizePlane,  sizePlane, -sizePlane), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));

//    vertex.append(VertexData(QVector3D(-sizePlane, -sizePlane, sizePlane), QVector2D(0.0f, 2.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
//    vertex.append(VertexData(QVector3D(sizePlane, -sizePlane, sizePlane), QVector2D(0.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
//    vertex.append(VertexData(QVector3D(-sizePlane, -sizePlane, -sizePlane), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
//    vertex.append(VertexData(QVector3D(sizePlane, -sizePlane, -sizePlane), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));

    vertex.append(VertexData(
                      QVector3D(-m_x, -m_y, m_z),
                      QVector2D(0.0f, 1.0f),
                      QVector3D(0.0, 1.0, 0.0)
                      ));
    vertex.append(VertexData(
                      QVector3D(m_x, -m_y, m_z),
                      QVector2D(0.0f, 0.0f),
                      QVector3D(0.0, 1.0, 0.0)
                      ));
    vertex.append(VertexData(
                      QVector3D(-m_x, -m_y, -m_z),
                      QVector2D(1.0f, 1.0f),
                      QVector3D(0.0, 1.0, 0.0)
                      ));
    vertex.append(VertexData(
                      QVector3D(m_x, -m_y, -m_z),
                      QVector2D(1.0f, 0.0f),
                      QVector3D(0.0, 1.0, 0.0)
                      ));

    QVector<GLuint> index;

    for (GLuint i = 0; i < 4; i += 4) {
        index.append(i + 0);
        index.append(i + 1);
        index.append(i + 2);
        index.append(i + 2);
        index.append(i + 1);
        index.append(i + 3);

    }

    m_objects.append(new SimpleObject3D(vertex, index, QImage(":/texture/ui_sklad/image_sklad_01.png")));
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePosition = static_cast<QVector2D>(event->localPos());
    }
    if (event->button() == Qt::RightButton) {
        qDebug() << "position x " << event->localPos();
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->pos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->pos());

    float angle = diff.length() / 2.0f;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    float mm = 0.05f;

    switch (event->buttons()) {
    case Qt::LeftButton:
        qDebug() << "Qt::LeftButton";
        m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));
        update();
        break;
    case Qt::RightButton:
        qDebug() << "Qt::RightButton";

        qreal position = event->localPos().x();

        if (position < event->localPos().x()) {
            qDebug() << "--";
        } else {
            qDebug() << "++";
        }

        qDebug() << position;

        break;
    }

    //m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        //m_z += 0.25f;
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
        update();
    }
    else if (event->delta() < 0) {
        //m_z -= 0.25f;
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
        update();
    }
    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "keyPressEvent, " << event->key();
    float mo = 0.05f;
    switch (event->key()) {
    case Qt::Key_Up:
        m_objects[0]->translate(QVector3D((mo += 0.005f), 0.0f, 0.0f));
        update();
        break;
    case Qt::Key_Down:
        m_objects[0]->translate(QVector3D(-(mo += 0.005f), 0.0f, 0.0f));
        update();
        break;
    case Qt::Key_Left:
        m_objects[0]->translate(QVector3D(0.0f, 0.0f, (mo += 0.005f)));
        update();
        break;
    case Qt::Key_Right:
        m_objects[0]->translate(QVector3D(0.0f, 0.0f, -(mo += 0.005f)));
        update();
        break;
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "keyReleaseEvent";
}











