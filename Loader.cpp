/*
 TCS2221 Computer Graphics
 Trimester 2, 2015/16
 Faculty of IT, Multimedia University

 CGLab01.cpp

 Objective: Lab01 Demo Models / World

 Copyright (C) 2011 by Ya-Ping Wong <ypwong@mmu.edu.my>

 This file (CGLab01.cpp) can be distributed to the students

 INSTRUCTIONS
 ============
 Please refer to CGLabmain.cpp for instructions

 CHANGE LOG
 ==========
*/

#include <GL/glut.h>
#include <string>
#include <fstream>
#include "Loader.hpp"
#include <sstream>

using namespace CGLab01;

void computeSurfaceNormal( GLfloat vertex1[],
                           GLfloat vertex2[],
                           GLfloat vertex3[],
                           GLfloat normal[])
{
    GLfloat v1[3], v2[3];
    v1[0] = vertex3[0] - vertex2[0];
    v1[1] = vertex3[1] - vertex2[1];
    v1[2] = vertex3[2] - vertex2[2];
    v2[0] = vertex1[0] - vertex2[0];
    v2[1] = vertex1[1] - vertex2[1];
    v2[2] = vertex1[2] - vertex2[2];
    normal[0] = v1[1]*v2[2] - v1[2]*v2[1];
    normal[1] = v1[2]*v2[0] - v1[0]*v2[2];
    normal[2] = v1[0]*v2[1] - v1[1]*v2[0];
    GLfloat magnitude = sqrt(   normal[0]*normal[0]
                                + normal[1]*normal[1]
                                + normal[2]*normal[2] );
    normal[0] /= magnitude;
    normal[1] /= magnitude;
    normal[2] /= magnitude;
}


void test(vector<GLfloat> vertices,vector<int> faces)
{
    ofstream fin;
    fin.open("test.txt");
    fin << vertices.size()<<" "<<faces.size()<<endl;
    for(int i=0;i<vertices.size();)
    {
        fin<<vertices[i++]<<" ";
        fin<<vertices[i++]<<" ";
        fin<<vertices[i++]<<endl;
    }
    for(int i=0;i<faces.size();)
    {
        fin<<faces[i++]<<" ";
        fin<<faces[i++]<<" ";
        fin<<faces[i++]<<endl;
    }
}
void MyModelLoader::load(string filename, float scale)
{
    vertices.clear();
    faces.clear();
    ifstream fin;
    fin.open(filename.c_str());
    while(fin.good())
    {
        string line;
        getline(fin,line);
        if(line[0]=='f')
        {
            stringstream ss;
            ss.str(line);
            ss>>line;
            int num;
            ss>>num;
            faces.push_back(num-1);
            ss>>num;
            faces.push_back(num-1);
            ss>>num;
            faces.push_back(num-1);
        }
        else if(line[0]=='v')
        {
            stringstream ss;
            ss.str(line);
            ss>>line;
            float num;
            ss>>num;
            vertices.push_back(num*scale);
            ss>>num;
            vertices.push_back(num*scale);
            ss>>num;
            vertices.push_back(num*scale);
        }
    }
    fin.close();
    vector<int>::iterator itr;
    GLfloat normal[3];

    if(filename=="data/logo2.obj")
    {
        logo = glGenLists(1);
        glNewList(logo, GL_COMPILE);
        glColor3f(1.f, 1.f, 1.f);
    }
    else
    {
        batman = glGenLists(1);
        glNewList(batman, GL_COMPILE);
        //glColor3f(0.4f, 0.4f, 0.4f);
    }

    int vcount;  //number of vertices for a particular polygon
    for(int i=0;i<faces.size();)
    {
        int i1 = faces[i++]*3;
        int i2 = faces[i++]*3;
        int i3 = faces[i++]*3;
        computeSurfaceNormal( &vertices[i1],&vertices[i2],&vertices[i3],normal );
        glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(&vertices[i1]);
        glVertex3fv(&vertices[i2]);
        glVertex3fv(&vertices[i3]);
        glEnd();
    }
    glEndList();
}

void MyModelLoader::drawBack()
{
    glDisable(GL_CULL_FACE);
    glColor3f(0.5f, 0.5f, 0.5f );
    glBegin(GL_QUADS);
    glVertex3f(50,-20,-1000);
    glVertex3f(-50,-20,-1000);
    glVertex3f(-50,-20,1000);
    glVertex3f(50,-20,1000);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f );
    glBegin(GL_QUADS);
    glVertex3f(-50,40,-1000);
    glVertex3f(-50,40,1000);
    glVertex3f(-50,-20,1000);
    glVertex3f(-50,-20,-1000);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f );
    glBegin(GL_QUADS);
    glVertex3f(50,40,-1000);
    glVertex3f(50,40,1000);
    glVertex3f(50,-20,1000);
    glVertex3f(50,-20,-1000);
    glEnd();

    glColor3f(0.5f, 0.5f, 0.5f );
    glBegin(GL_QUADS);
    glVertex3f(50,40,-1000);
    glVertex3f(-50,40,-1000);
    glVertex3f(-50,40,1000);
    glVertex3f(50,40,1000);
    glEnd();
    glEnable(GL_CULL_FACE);
}

void MyModelLoader::draw()
{
    glTranslatef(0,-20,0);
    glDisable(GL_CULL_FACE);
    glCallList(batman);
    glEnable(GL_CULL_FACE);
}

void MyModelLoader::drawLogo(float rot)
{
    glTranslatef(0,20,0);
    glRotatef(90,1.f,0.f,0.f);
    glRotatef(rot,0.f,0.f,1.0f);

    glDisable(GL_CULL_FACE);
    glCallList(logo);
    glEnable(GL_CULL_FACE);
}

void MyVirtualWorld::draw()
{
    glPushMatrix();
    mymodelloader.draw();
    glPopMatrix();

    glPushMatrix();
    mymodelloader.drawLogo(angel);
    glPopMatrix();
    mymodelloader.drawBack();
}
