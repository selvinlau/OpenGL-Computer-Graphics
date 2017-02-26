#include "mainview.h"
#include "math.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    delete shapeModel;

    // Free Buffer Objects before Vertex Arrays
    glDeleteBuffers(1,&shapeBO);
    glDeleteBuffers(1,&colorBO);
    glDeleteVertexArrays(1,&VAO);

    // Free the main shader
    delete mainShaderProg;

    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

/**
 * @brief MainView::createShaderPrograms
 *
 * Creates the shaderprogram for OpenGL rendering
 */
void MainView::createShaderPrograms() {
    // Qt wrapper (way cleaner than using pure OpenGL)
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    mainShaderProg->link();

    /* Add your other shaders below */
    modelPtr = glGetUniformLocation(mainShaderProg->programId(),"model");
    viewPtr = glGetUniformLocation(mainShaderProg->programId(),"view");
    projPtr = glGetUniformLocation(mainShaderProg->programId(),"proj");
    /* End of custom shaders */

    // Store the locations (pointers in gpu memory) of uniforms in Glint's

}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    // TODO: implement buffer creation
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&shapeBO);
    glGenBuffers(1,&colorBO);



    //Orders of Binding, Enabling and setting Vertex Attrib
    //Does affect the outcome
    glBindBuffer(GL_ARRAY_BUFFER,shapeBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,colorBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArray(0);

}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    shapeModel = new Model(filename);
    numTris = shapeModel->getNumTriangles();

    // TODO: implement loading of model into Buffer Objects
    QVector<QVector3D> vectorList;
    QVector<QVector3D> colorList;
    QVector<QVector3D> normalList;
    vectorList = shapeModel->getVertices();
    for(int i = 0; i < numTris;i++){

        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        QVector3D color;
        color.setX(r);
        color.setY(g);
        color.setZ(b);
        colorList.append(color);
        colorList.append(color);
        colorList.append(color);

    }


    glBindBuffer(GL_ARRAY_BUFFER,bufferObject);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vectorList.size()*3,vectorList.data(),GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,colorBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*colorList.size()*3,colorList.data(),GL_DYNAMIC_DRAW);



}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance
    qDebug()<<"Update Buff";
}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions
    qDebug()<<"Update uniform";
}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    //glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */

    createShaderPrograms();

    createBuffers();
    loadModel(":/models/cube.obj", shapeBO);

    // For animation, you can start your timer here

}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {

    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {

    // Clear the screen before rendering
    glClearColor(0.0f,0.0f,0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();


    // TODO: implement your drawing functions

    //move 4 units
    view.translate(0.0,0.0,-4.0);
    // FOV 60 degree
    proj.perspective(60,1.0,2,6);

    model.scale(scaleValue);

    model.rotate(rotationXYZ.x(),1,0,0);
    model.rotate(rotationXYZ.y(),0,1,0);
    model.rotate(rotationXYZ.z(),0,0,1);

    //Push to GPU
    glUniformMatrix4fv(modelPtr,1,GL_FALSE,model.data());
    glUniformMatrix4fv(viewPtr,1,GL_FALSE,view.data());
    glUniformMatrix4fv(projPtr,1,GL_FALSE,proj.data());

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES,0,shapeModel->getVertices().size());
    mainShaderProg->release();
}

// Add your function implementations below

// TODO: add your code
