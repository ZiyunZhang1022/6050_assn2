//
//		          Programming Assignment #1
//
//			        Victor Zordan
//
//
//
/***************************************************************************/

/* Include needed files */
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <math.h>
#include "main.h"
#include <GLUT/GLUT.h>   // The GL Utility Toolkit (Glut) Header
#include "z_buffer.h"
#include "transfer.h"

#define WIDTH 500
#define HEIGHT 500
using namespace std;

int x_last,y_last;
scanning_zbuffer myzbuffer;
PointTransfer PointTransfer;
bool g_rotate = false;
GLfloat pixel[winX][3];
int tag;
bool isWireframe = false;
vector<char> command;
model myModel;
int countz = 0;

/***************************************************************************/



/***************************************************************************/

void write_pixel(int x, int y, int z, double intensity)
/* Turn on the pixel found at x,y */
{
    
    glColor3f (intensity, intensity, intensity);
    glBegin(GL_POINTS);
    printf(" x,y,z is (%d,%d,%d)\n", x,y,z);
    glVertex3i( x, y, z);
    glEnd();
}

/***************************************************************************/

void drawLine(vector3f lBegin, vector3f lEnd) {
    float xBegin = lBegin.x;
    float yBegin = lBegin.y;
    float zBegin = lBegin.z;
    float xEnd = lEnd.x;
    float yEnd = lEnd.y;
    float zEnd = lEnd.z;
    
    float dx = xEnd - xBegin;
    float dy = yEnd - yBegin;
    float dz = zEnd - zBegin;
    
    if(xBegin < xEnd) {
        for(float x = xBegin; x < xEnd; x += 0.1) {
            float k = (x - xBegin) / dx;
            float y = dy * k + yBegin;
            float z = dz * k + zBegin;
            write_pixel(round(x), round(y), round(z), 1.0);
        }
    }else if(xBegin > xEnd) {
        for(float x = xBegin; x > xEnd; x -= 0.1) {
            float k = (x - xBegin) / dx;
            float y = dy * k + yBegin;
            float z = dz * k + zBegin;
            write_pixel(round(x), round(y), round(z), 1.0);
        }
    }else {
        if(yBegin < yEnd) {
            for(float y = yBegin; y < yEnd; y += 0.1){
                float k = (y - yBegin) / dy;
                float z = dz * k + zBegin;
                write_pixel(round(xBegin), round(y), round(z), 1.0);
            }
        }else if(yBegin > yEnd) {
            for(float y = yBegin; y > yEnd; y -= 0.1){
                float k = (y - yBegin) / dy;
                float z = dz * k + zBegin;
                write_pixel(round(xBegin), round(y), round(z), 1.0);
            }
        }else {
            if(zBegin < zEnd) {
                for(float z = zBegin; z < zEnd; z += 1) {
                    write_pixel(round(xBegin), round(yBegin), round(z), 1.0);
                }
            }else {
                for(float z = zBegin; z > zEnd; z -= 1) {
                    write_pixel(round(xBegin), round(yBegin), round(z), 1.0);
                }
            }
        }
    }
    write_pixel(round(xEnd), round(yEnd), round(zEnd), 1.0);
}

/***************************************************************************/


void init_window()
/* Clear the image area, and set up the coordinate system */
{
    
    /* Clear the window */
//   glClearColor(0.0,0.0,0.0,0.0);
//    glShadeModel(GL_SMOOTH);
//    glOrtho(0,WIDTH,0,HEIGHT,-500.0,500.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500.0,(GLfloat)winX,-500.0,(GLfloat)winY,-500.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    
}


//***************************************************************************/

void display ( void )   // Create The Display Function
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen
    
   //    drawLine(myModel.vertex[0], myModel.vertex[1]);//<-you can get rid of this call if you like
    // CALL YOUR CODE HERE
    
    if (!isWireframe) {
        myzbuffer.CreateClassTable();
        myzbuffer.initBuffer();
        for(int y = winY-1;y >= -winY;y--) {
          
        myzbuffer.initBuffer();
        myzbuffer.addPolTable(y);
        tag=myzbuffer.getActPolNum();
        if(myzbuffer.getActPolNum()) {
            myzbuffer.updateActSideTable(y);
            //tag++;
        }
        //pixel=myzbuffer.m_color_buffer;
        //glRasterPos2i(-winX/2,-winY/2+y);
        //if(tag>5)break;
        glRasterPos2i(0,y);
        //write_pixel(int x, <#int y#>, <#int z#>, <#double intensity#>)
        glDrawPixels(winX,1,GL_RGB,GL_FLOAT,&myzbuffer.m_color_buffer[0][0]);
        
        
        
    }
}
    
    for(int i = 0;i < myzbuffer.getModel().faceCount; i++) {
        int vexIndex0 = myzbuffer.getModel().faceVertex[i].vertexIndex[0] - 1;
        int vexIndex1 = myzbuffer.getModel().faceVertex[i].vertexIndex[1] - 1;
        int vexIndex2 = myzbuffer.getModel().faceVertex[i].vertexIndex[2] - 1;

        vector3f vex0 = myzbuffer.getModel().vertex[vexIndex0];
        vector3f vex1 = myzbuffer.getModel().vertex[vexIndex1];
        vector3f vex2 = myzbuffer.getModel().vertex[vexIndex2];
        
        drawLine(vex0, vex1);
        drawLine(vex0, vex2);
        drawLine(vex1, vex2);
                 
    }
    glFlush();
    
    glutSwapBuffers();                                      // Draw Frame Buffer
}

/***************************************************************************/
void mouse(int button, int state, int x, int y)
{
    /* This function I finessed a bit, the value of the printed x,y should
     match the screen, also it remembers where the old value was to avoid multiple
     readings from the same mouse click.  This can cause problems when trying to
     start a line or curve where the last one ended */
    static int oldx = 0;
    static int oldy = 0;
    int mag;
    
    y *= -1;  //align y with mouse
    y += 500; //ignore
    mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
    if (mag > 20) {
        printf(" x,y is (%d,%d)\n", x,y);
    }
    oldx = x;
    oldy = y;
    x_last = x;
    y_last = y;
}

/***************************************************************************/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
    int size = myzbuffer.getModel().vertexCount;
    switch ( key ) {
        case 27:              // When Escape Is Pressed...
            exit ( 0 );   // Exit The Program
            break;
        case '1':             // stub for new screen
            printf("New screen\n");
            break;
        case 'w':
            if(command.size() != 0){
                if(command.at(0) == 't') {
                    PointTransfer.translation(myzbuffer.getModel().vertex, false, false, true, false, 10, size);
                    myzbuffer.clearAll();
                    command.clear();
                    
                }
                else if(command.at(0) == 'r') {
                    PointTransfer.rotation(myzbuffer.getModel().vertex, false, true, 360 - 30, size);
                    myzbuffer.clearAll();
                    command.clear();
                }else if(command.at(0) == 'e'){
                    PointTransfer.scale(myzbuffer.getModel().vertex, true, false, 0.1, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
            }else {
            if(isWireframe) isWireframe = false;
            else isWireframe = true;
            }
            break;
        case 's' :
            if(command.size() != 0){
                if(command.at(0) == 't') {
                    PointTransfer.translation(myzbuffer.getModel().vertex, false, false, false, true, -10, size);
                    myzbuffer.clearAll();
                    command.clear();
                } else if(command.at(0) == 'r') {
                    PointTransfer.rotation(myzbuffer.getModel().vertex, false, true, 30, size);
                    myzbuffer.clearAll();
                    command.clear();
                } else if(command.at(0) == 'e'){
                    PointTransfer.scale(myzbuffer.getModel().vertex, false, true, 0.1, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
            }
            break;
        case 'a' :
            if(command.size() != 0){
                if(command.at(0) == 't') {
                    PointTransfer.translation(myzbuffer.getModel().vertex, false, true, false, false, -10, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
                else if(command.at(0) == 'r') {
                    PointTransfer.rotation(myzbuffer.getModel().vertex, true, false, 360 - 30, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
            }
            break;
        case 'd' :
            if(command.size() != 0){
                if(command.at(0) == 't') {
                    PointTransfer.translation(myzbuffer.getModel().vertex, true, false, false, false, 10, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
                else if(command.at(0) == 'r') {
                    PointTransfer.rotation(myzbuffer.getModel().vertex, true, false, 30, size);
                    myzbuffer.clearAll();
                    command.clear();
                }
            }
            break;
        case 'r':
            command.push_back('r');
            break;
        case 't':
            command.push_back('t');
            break;
        case 'e':
            command.push_back('e');
        default:
            break;
    }
}
/***************************************************************************/

void reshape(int w,int h)
{
    //winX=w;
    //winY=h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

int main (int argc, char *argv[])
{
    /* This main function sets up the main loop of the program and continues the
     loop until the end of the data is reached.  Then the window can be closed
     using the escape key.						  */
    
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize  ( winX,winY );
    glutCreateWindow    ( "Computer Graphics" );
    glutDisplayFunc     ( display );
    glutIdleFunc	    ( display);
    glutReshapeFunc(reshape);
    glutMouseFunc       ( mouse );
    glutKeyboardFunc    ( keyboard );
    
    init_window();				             //create_window
    
    glutMainLoop        ( );                 // Initialize The Main Loop
}


