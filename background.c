
#include <GL/glut.h>
#include "global.h"

static int bgBirthFrame;

// 背景の初期化
void initBackground(void){
    bgBirthFrame = frame_count;
}

// 背景の描画
void renderBackground(void){
    int f = (frame_count - bgBirthFrame) % 200;
    double d = 10.0 / 200;

    glDisable(GL_LIGHTING);

    int i,j;
    for(i=-3;i<4;i++){
        for(j=-3;j<4;j++){
            glPushMatrix();
            glColor3d(0.0,1.0,0.0);
            glTranslatef(i*10.0-d*f,j*10.0,-10.0);
            glutSolidCube(4);
            glPopMatrix();

            glPushMatrix();
            glColor3d(0.0,0.0,1.0);
            glTranslatef(i*10.0-d*f,j*10.0-5.0,-10.0);
            glutSolidCube(4);
            glPopMatrix();

            glPushMatrix();
            glColor3d(0.0,0.0,1.0);
            glTranslatef(i*10.0-d*f+5.0,j*10.0,-10.0);
            glutSolidCube(4);
            glPopMatrix();

            glPushMatrix();
            glColor3d(0.0,1.0,0.0);
            glTranslatef(i*10.0-d*f+5.0,j*10.0-5.0,-10.0);
            glutSolidCube(4);
            glPopMatrix();
        }
    }
    glEnable(GL_LIGHTING);
}

// 背景の終了
void endBackground(void){

}
