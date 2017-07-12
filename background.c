#include <GL/glut.h>
#include <math.h>
#include "global.h"

#include <stdio.h> //debug

static int bgBirthFrame;

#ifndef M_PI
#define M_PI 3.14159265
#endif // M_PI

#ifndef M_PI_2
#define M_PI_2 1.57079633
#endif // M_PI_2


// 背景の初期化

void initBackground(void){
    bgBirthFrame = frame_count;
}

// 背景の描画
void renderBackground(void){
       int f = (frame_count - bgBirthFrame) % 180;
    double c = cos(f*M_PI/180.0);
    double s = sin(f*M_PI/180.0);

    double torque = 360.0*c;
    
    //printf("f:%d c:%f torque:%f \n", f, c, torque); //for debugging printf
    
    glDisable(GL_LIGHTING);
    
    int i,j;
    double x,z;
    for(i=1; i<73; i++){
        x = cos(i*5.0*M_PI/180.0);
        z = sin(i*5.0*M_PI/180.0);
        for(j=-9; j<12; j++){
	    glPushMatrix();
            glColor3d(0.0,fabs(s),fabs(c));
	    glTranslatef(x*400.0, j*40.0, z*400.0);
	    glRotatef(torque+j*90.0, 0.0, 1.0, 0.0);
            glutSolidCube(14);
            glPopMatrix();

            glPushMatrix();
            glColor3d(0.0,0.0,1.0);
            glTranslatef(x*400.0, j*40.0-20.0, z*400.0);
            glutSolidCube(14);
            glPopMatrix();
        }
    }    
    glEnable(GL_LIGHTING);
}

// 背景の終了
void endBackground(void){

}
