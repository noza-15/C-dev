﻿
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"

// hugaが生成される時に実行される関数
static double* huga_init(double z) {
    double* p;
    p = malloc(sizeof(double) * 7);
    p[0] = 15;//敵の座標
    p[1] = 3.14*rand() / RAND_MAX;//移動の傾き
	p[2] = 10.0*rand() / RAND_MAX - 5;//z座標
    p[3] = 0;//xの判定開始地点
    p[4] = 0;//xの判定終了地点
    p[5] = 0;//zの判定開始地点
    p[6] = 0;//zの判定終了地点
    return p;
}

static void render_crash_wire(double* param) {
	glDisable(GL_LIGHTING);
	glColor3d(1.0, 1.0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3d(param[0] + param[3], param[5], 0);
	glVertex3d(param[0] + param[3], param[6], 0);
	glVertex3d(param[0] + param[4], param[6], 0);
	glVertex3d(param[0] + param[4], param[5], 0);
	glEnd();
	glEnable(GL_LIGHTING);

}

// hugaを描画する関数
static void huga_render(int birth, double* param, double z) {
    param[0] = 15 - 0.3*(frame_count - birth);
    glPushMatrix();
    glTranslatef(param[0], param[2], 0.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glColor3d(0.3, 1.0, 0.3);
    glScalef(4 + 3.8*sin(1.0*(frame_count - birth) / 8 + param[1]), 0.5, 0.5);
    //if (frame_count % 3 == 1)printf("%6.4lf\t%6.4lf\n", param[0], param[2]);
    glutSolidCube(1);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    render_crash_wire(param);
}

// hugaがプレイヤーと衝突したか調べる関数
static int huga_judge(int birth, double* param, double z) {

    param[3] = -2 - 1.8*sin(1.0*(frame_count - birth) / 8 + param[1]);
    param[4] = 2 + 1.8*sin(1.0*(frame_count - birth) / 8 + param[1]);

    param[5] = param[2] - 0.4;
    param[6] = param[2] + 0.4;

    if (param[0]-0.5<param[4] && param[0]+0.5>param[3] && z>param[5] && z<param[6])return 1;
    return 0;
}

// hugaを削除する時に実行する関数
static void huga_delete(int birth, double* param, double z) {

}

// hugaという障害物の定義を返す関数
Obstacle getHugaDefinition(void) {
    // 実際の障害物定義の例
    Obstacle huga = {
        huga_init,
        huga_render,
        huga_judge,
        huga_delete
    };
    return huga;
}
