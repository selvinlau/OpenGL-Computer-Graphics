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
    if(setting == 0){
        glDeleteBuffers(1,&colorBO);
    }else if(setting == 1){
        glDeleteBuffers(1,&normalBO);
    }
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
    settingPtr = glGetUniformLocation(mainShaderProg->programId(),"setting");

    if(setting == 1){
        materialColorPtr = glGetUniformLocation(mainShaderProg->programId(),"materialColor");
        normalPtr = glGetUniformLocation(mainShaderProg->programId(),"normal");
        lightPosPtr = glGetUniformLocation(mainShaderProg->programId(),"lightPos");
        intensityPtr = glGetUniformLocation(mainShaderProg->programId(),"intensity");
        positionPtr = glGetUniformLocation(mainShaderProg->programId(),"position");
        camPtr = glGetUniformLocation(mainShaderProg->programId(),"cam");

    }
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
    //Generating/Binding VertexArray buffer
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //Generating/Binding shape buffer
    glGenBuffers(1,&shapeBO);
    glBindBuffer(GL_ARRAY_BUFFER,shapeBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    if(setting == 0){
        //Generating/Binding color buffer
        glGenBuffers(1,&colorBO);
        glBindBuffer(GL_ARRAY_BUFFER,colorBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    }
    else if(setting == 1){
        //Generating/Binding normal buffer
        glGenBuffers(1,&normalBO);
        glBindBuffer(GL_ARRAY_BUFFER,normalBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
    }

    //UnBinding VertexArray buffer
    glBindVertexArray(0);

}

void MainView::loadModel(QString filename, GLuint bufferObject) {
    shapeModel = new Model(filename);
    numTris = shapeModel->getNumTriangles();

    // TODO: implement loading of model into Buffer Objects

    //Preparing vector List to store values
    QVector<QVector3D> vectorList;
    QVector<QVector3D> colorList;
    QVector<QVector3D> normalList;
    vectorList = shapeModel->getVertices();

    //pushing of data to GPU using shape buffer pointer
    glBindBuffer(GL_ARRAY_BUFFER,bufferObject);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vectorList.size()*3,vectorList.data(),GL_DYNAMIC_DRAW);

    if(setting == 0){
        for(uint i = 0; i < numTris;i++){

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

        //pushing of data to GPU using colorBO pointer
        glBindBuffer(GL_ARRAY_BUFFER,colorBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*colorList.size()*3,colorList.data(),GL_DYNAMIC_DRAW);
    }
    else if(setting == 1){

        normalList = shapeModel->getNormals();

        //pushing of data to GPU using normal pointer
        glBindBuffer(GL_ARRAY_BUFFER,normalBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*normalList.size()*3,normalList.data(),GL_DYNAMIC_DRAW);

    }


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
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */

    createShaderPrograms();

    createBuffers();
    if(setting == 0){
        loadModel(":/models/cube.obj", shapeBO);
    }else if(setting == 1){
        loadModel(":/models/sphere.obj", shapeBO);
    }

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

    proj.setToIdentity();
    if(setting == 0){
        proj.perspective(60,(float)newWidth/(float)newHeight,2,6);
    }
    else if(setting == 1){
        proj.perspective(30.0,(float)newWidth/(float)newHeight,50.0,1600.0);
    }
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

    //Bind the Vertex Array before drawing
    glBindVertexArray(VAO);

    //Pushing of setting value into the shader to ensure correct usage of values
    glUniform1i(settingPtr,setting);

    if(setting == 0){
        model.setToIdentity();
        view.setToIdentity();

        //move 4 units
        view.translate(0.0,0.0,-4.0);

        //Scaling base on Scalevalue returned from user_input. Default value is 1
        model.scale(scaleValue);

        //Rotation around their x y and z axis
        model.rotate(rotationXYZ.x(),1,0,0);
        model.rotate(rotationXYZ.y(),0,1,0);
        model.rotate(rotationXYZ.z(),0,0,1);

        //Push to GPU
        glUniformMatrix4fv(modelPtr,1,GL_FALSE,model.data());
        glUniformMatrix4fv(viewPtr,1,GL_FALSE,view.data());
        glUniformMatrix4fv(projPtr,1,GL_FALSE,proj.data());


        //Start drawing
        glDrawArrays(GL_TRIANGLES,0,shapeModel->getVertices().size());
    }else if(setting == 1){
        renderRaytracerScene();
    }
    mainShaderProg->release();
}

// Add your function implementations below

// TODO: add your code
