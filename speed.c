
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"
/* 障害物定義の雛形のようなもの */

//double user_z = 0;

// speedが生成される時に実行される関数
static double* speed_init(double z) {
    double* p;
    p = malloc(sizeof(double) * 3);
    p[0] = 15;//敵の座標
    p[1] = 0;//移動の傾き
    p[2] = 6.0*rand() / RAND_MAX - 3;//z座標の初期値
    return p;
}

// speedを描画する関数
static void speed_render(int birth, double* param, double z) {

    param[0] = 15-0.2*(frame_count-birth) - 1*sin(1.0*(frame_count - birth) / 10);

    glPushMatrix();
    glTranslatef(param[0], param[2], 0.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glColor3d(0.6, 0.4, 1.0);
    glScalef(0.7, 0.7, 0.7);
    glutSolidOctahedron();
    glEnable(GL_LIGHTING);
    glPopMatrix();

}

// speedがプレイヤーと衝突したか調べる関数
static int speed_judge(int birth, double* param, double z) {
    if (param[0]<0.5 && param[0]>-0.5 && z>param[2] - 0.4 && z<param[2] + 0.4)return 1;
    return 0;
}

// speedを削除する時に実行する関数
static void speed_delete(int birth, double* param, double z) {

}

// speedという障害物の定義を返す関数
Obstacle getSpeedDefinition(void) {
    //実際の障害物定義の例
    Obstacle speed = {
        speed_init,
        speed_render,
        speed_judge,
        speed_delete
    };
    return speed;
}
