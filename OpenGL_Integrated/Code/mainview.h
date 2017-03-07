#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <QImage>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    /* Add your public members below */
    QVector3D rotationXYZ;// vector used for storing XYZ variables
    float scaleValue = 1;
    float startX;// capture x starting position
    float startY;// capture y starting position

    void loadTexture(QString file, GLuint texPtr);
    QVector<quint8> imageToBytes(QImage image);
    /* End of public members */

    QVector3D convertHSLtoRGB(float H, float S, float L);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();

    // Raytracer scene functions
    void renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos);
    void renderRaytracerScene();

    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);
    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;

    QTimer timer; // timer used for animation

    //Change settings to obtain desire results
    //0 = cube
    //1 = sphere
    GLint setting = 1;

    //buffer pointer for GPU
    Model *shapeModel;
    GLuint shapeBO;
    GLuint VAO;
    GLuint colorBO;
    GLuint normalBO;

    //Pointers for shader
    GLuint modelPtr;
    GLuint viewPtr;
    GLuint projPtr;
    GLuint normalPtr;
    GLuint materialColorPtr;
    GLuint intensityPtr;
    GLuint lightPosPtr;
    GLuint positionPtr;
    GLuint settingPtr;
    GLuint camPtr;
    GLuint texturePtr;

    //Extra uniform
    GLint extra;

    //Variable for handling transformation
    QMatrix3x3 normal;
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 proj;

    unsigned numTris;


    /* End of private members */

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
