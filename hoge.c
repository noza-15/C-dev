

#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"
/* 障害物定義の雛形のようなもの */

// hogeが生成される時に実行される関数
static double* hoge_init(double z) {
    double* p;
    p = malloc(sizeof(double) * 7);
    p[0] = 15;//敵の座標
    p[1] = 3.14*rand() / RAND_MAX;//移動の傾き
	p[2] = 10.0*rand() / RAND_MAX - 5;//z座標の初期値
    p[3] = 0;//物体自体のxの判定開始地点
    p[4] = 0;//物体自体のxの判定終了地点
    p[5] = 0;//物体自体のzの判定開始地点
    p[6] = 0;//物体自体のzの判定終了地点
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

// hogeを描画する関数
static void hoge_render(int birth, double* param, double z) {
    param[0] = 15 - 0.3*(frame_count - birth);
    param[2] = 2*sin(1.0*(frame_count - birth) / 10 + param[1]);
    glPushMatrix();

    glTranslatef(param[0], param[2], 0.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glColor3d(0.3, 0.3, 1.0);
    glScalef(0.7, 0.7, 0.7);
    //if (frame_count % 3 == 1)printf("%6.4lf\t%6.4lf\n", param[0], param[2]);
    glutSolidOctahedron();
    glEnable(GL_LIGHTING);
    glPopMatrix();
    render_crash_wire(param);
}

// hogeがプレイヤーと衝突したか調べる関数
static int hoge_judge(int birth, double* param, double z) {

    param[3] = - 0.5;
    param[4] = + 0.5;

    param[5] = param[2] - 0.4;
    param[6] = param[2] + 0.4;

    if (param[0]-0.5<param[4] && param[0]+0.5>param[3] && z > param[5] && z < param[6])return 1;
    return 0;
}

// hogeを削除する時に実行する関数
static void hoge_delete(int birth, double* param, double z) {

}

// hogeという障害物の定義を返す関数
Obstacle getHogeDefinition(void) {
    // 実際の障害物定義の例
    Obstacle hoge = {
        hoge_init,
        hoge_render,
        hoge_judge,
        hoge_delete
    };
    return hoge;
}
