
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"

// missileが生成される時に実行される関数
static double* missile_init(double z) {
	double* p;
	p = malloc(sizeof(double) * 8);
	p[0] = 15;//敵の座標
	p[1] = 3.14*rand() / RAND_MAX;//移動の傾き
	p[2] = 6.0*rand() / RAND_MAX - 3;//z座標
	p[3] = 0;//xの判定開始地点(差分)
	p[4] = 0;//xの判定終了地点(差分)
	p[5] = 0;//zの判定開始地点
	p[6] = 0;//zの判定終了地点
	p[7] = p[2];//z座標の初期値
	return p;
}

static void render_crash_wire(double* param) {
	glBegin(GL_LINE_LOOP);
	glVertex3d( param[3], param[5], 0);
	glVertex3d(param[3], param[6], 0);
	glVertex3d(param[4], param[6], 0);
	glVertex3d(param[4], param[5], 0);
	glEnd();

}

// missileを描画する関数
static void missile_render(int birth, double* param, double z) {
	int start_frame = 30;
	int end_frame = 40;

	param[0] = 15-0.3*(frame_count-birth);
	if (frame_count - birth > start_frame)param[0] = 15 - 0.3 * start_frame;
	if (frame_count - birth > end_frame) {
		param[0] = 15 - 0.3*(frame_count - birth - end_frame + start_frame);
		param[2] = param[7]-0.3*param[1]*(frame_count-birth-end_frame);
	}
	glPushMatrix();
	glTranslatef(param[0], param[2], 0.0);
	glDisable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glColor3d(1, 0.4, 0.4);
	glScalef(0.3, 0.3, 0.3);
	glutSolidDodecahedron();
	glEnable(GL_LIGHTING);
	glPopMatrix();

	if (frame_count - birth == end_frame) {
		param[1] = (param[2]-z) / param[0];
		printf("here");
	}
	render_crash_wire(param);
}

// missileがプレイヤーと衝突したか調べる関数
static int missile_judge(int birth, double* param, double z) {

	param[3] = param[0]+ 0.4;
	param[4] = param[0]- 0.4;
	param[5] = param[2] - 0.4;
	param[6] = param[2] + 0.4;

	if (param[4]<0 && param[3]>0 && z>param[5] && z<param[6])return 1;
	return 0;
}

// missileを削除する時に実行する関数
static void missile_delete(int birth, double* param, double z) {

}

// missileという障害物の定義を返す関数
Obstacle getMissileDefinition(void) {
	// 実際の障害物定義の例
	Obstacle missile = {
		missile_init,
		missile_render,
		missile_judge,
		missile_delete
	};
	return missile;
}
