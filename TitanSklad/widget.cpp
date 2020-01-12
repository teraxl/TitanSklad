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

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

GLWidget::~GLWidget()
{
    delete &m_camera;

    for (int i = 0; i < m_objects.size(); ++i) {
        delete &m_objects[i];
    }
    for (int i = 0; i < m_groups.size(); ++i) {
        delete &m_groups[i];
    }
    for (int i = 0; i < m_TransformObjects.size(); ++i) {
        delete &m_TransformObjects[i];
    }
}

void GLWidget::initializeGL()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    float step = 2.0f;
    createObject();
}

void GLWidget::resizeGL(int w, int h)
{
    float aspect = static_cast<float>(w) / static_cast<float>(h);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(25.0f, aspect, 0.01f, 10000.0f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_programs.bind();
    m_programs.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_programs.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_programs.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&m_programs);

    for (int i = 0; i < m_TransformObjects.size(); ++i) {
        m_TransformObjects[i]->draw(&m_programs, context()->functions());
    }


}

void GLWidget::initShaders()
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

void GLWidget::initCube(float width)
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

void GLWidget::initPlane(float size)
{
    float sizePlane = 2.0f; //size * 1.0f;
    float m_x = 23.0f;
    float m_y = 1.5f;
    float m_z = 50.0f;
    QVector<VertexData> vertex;

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

void GLWidget::createObject()
{
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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePosition = static_cast<QVector2D>(event->localPos());
    }
    if (event->button() == Qt::RightButton) {
        m_mousePosition = static_cast<QVector2D>(event->localPos());
        qDebug() << "position x " << event->localPos();
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    float angleX = diff.y() / 2.0f;
    float angleY = diff.x() / 2.0f;

    switch (event->buttons()) {
    case Qt::LeftButton:
        m_camera->translate(QVector3D(angleY, 0.0f, 0.0f));
        m_camera->translate(QVector3D(0.0f, 0.0f, angleX));
        update();
        break;
    case Qt::RightButton:

        m_camera->rotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));
        m_camera->rotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));
        update();
        break;
    }
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.5f));
        update();
    }
    else if (event->delta() < 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.5f));
        update();
    }
    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    float mo = 0.05f;
    switch (event->key()) {
    case Qt::Key_Up:
        m_objects[0]->translate(QVector3D(-(mo += 0.05f), 0.0f, 0.0f));
        update();
        break;
    case Qt::Key_Down:
        m_objects[0]->translate(QVector3D((mo += 0.05f), 0.0f, 0.0f));
        update();
        break;
    case Qt::Key_Left:
        m_objects[0]->translate(QVector3D(0.0f, 0.0f, (mo += 0.05f)));
        update();
        break;
    case Qt::Key_Right:
        m_objects[0]->translate(QVector3D(0.0f, 0.0f, -(mo += 0.05f)));
        update();
        break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{

}


