#include "mainview.h"

void MainView::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    // OpenGL assignment 1, part 2: create a function to render the sphere
    // Use Model(":/models/sphere.obj") for the model

    model.setToIdentity();
    view.setToIdentity();

    // you must remove these Q_UNUSED when you implement this function
    view.translate(-200,-200,-1000);

    model.rotate(rotationXYZ.x(),200,0,0);
    model.rotate(rotationXYZ.y(),0,200,0);
    model.rotate(rotationXYZ.z(),0,0,200);

    model.translate(pos);

    model.scale(scaleValue);


    normal = model.normalMatrix();
    lightpos = model * lightpos;
    glUniform3f(lightPosPtr,lightpos.x(),lightpos.y(),lightpos.z());
    glUniform3f(camPtr,-200,-200,-1000);
    glUniform3f(positionPtr,pos.x(),pos.y(),pos.z());
    glUniform4f(intensityPtr,material.x(),material.y(),material.z(),material.w());

    glUniformMatrix4fv(modelPtr,1,GL_FALSE,model.data());
    glUniformMatrix4fv(projPtr,1,GL_FALSE,proj.data());
    glUniformMatrix4fv(viewPtr,1,GL_FALSE,view.data());
    glUniformMatrix3fv(normalPtr,1,GL_FALSE,normal.data());

    glUniform3f(materialColorPtr,color.x(),color.y(),color.z());
    glDrawArrays(GL_TRIANGLES,0,shapeModel->getVertices().size());
}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderRaytracerScene()
{
    QVector3D lightpos = QVector3D(-200,600,1500);

    // Blue sphere
    renderSphere(QVector3D(90,320,100),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Green sphere
    renderSphere(QVector3D(210,270,300),QVector3D(0,1,0),QVector4D(0.2f,0.3f,0.5f,8),lightpos);

    // Red sphere
    renderSphere(QVector3D(290,170,150),QVector3D(1,0,0),QVector4D(0.2f,0.7f,0.8f,32),lightpos);

    // Yellow sphere
    renderSphere(QVector3D(140,220,400),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos);

    // Orange sphere
    renderSphere(QVector3D(110,130,200),QVector3D(1,0.5f,0),QVector4D(0.2f,0.8f,0.5f,32),lightpos);
}
