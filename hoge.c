

#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"
/* 障害物定義の雛形のようなもの */


// hogeが生成される時に実行される関数
static double* hoge_init(void) {
	double* p;
	p = malloc(sizeof(double) * 7);
	p[0] = 15;//敵の座標
	p[1] = 3.14*rand() / RAND_MAX;//移動の傾き
	p[2] = 6.0*rand() / RAND_MAX - 3;//z座標の初期値
	p[3] = 0;//xの判定開始地点
	p[4] = 0;//xの判定終了地点
	p[5] = 0;//zの判定開始地点
	p[6] = 0;//zの判定終了地点
	return p;
}
static void render_crash_wire(double* param) {
	glBegin(GL_LINE_LOOP);
	glVertex3d(param[0] + param[3], param[5], 0);
	glVertex3d(param[0] + param[3], param[6], 0);
	glVertex3d(param[0] + param[4], param[6], 0);
	glVertex3d(param[0] + param[4], param[5], 0);
	glEnd();

}

// hogeを描画する関数
static void hoge_render(int birth, double* param) {
	param[0] -= 0.1;
	param[2] -= 0.1 * sin(1.0*(frame_count - birth) / 10 +param[1]);
	glPushMatrix();

	glTranslatef(param[0], param[2], 0.0);
	glDisable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glColor3d(0.6, 0.4, 1.0);
	glScalef(0.7, 0.7, 0.7);
	//if (frame_count % 3 == 1)printf("%6.4lf\t%6.4lf\n", param[0], param[2]);
	glutSolidOctahedron();
	glEnable(GL_LIGHTING);
	glPopMatrix();
	render_crash_wire(param);
}

// hogeがプレイヤーと衝突したか調べる関数
static int hoge_judge(int birth, double* param, double z) {

	//あんまりよくない実装だけど…操作してる物体のz座標の持ってき方がわからなかった
	user_z = z;
	param[3] = - 0.5;
	param[4] = + 0.5;

	param[5] = param[2] - 0.4;
	param[6] = param[2] + 0.4;

	if (param[0]-0.5<param[4] && param[0]+0.5>param[3] && z > param[5] && z < param[6])return 1;
	return 0;
}

// hogeを削除する時に実行する関数
static void hoge_delete(int birth, double* param) {

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
//
//#include <math.h>
//#include <stdlib.h>
//#include <GL/glut.h>
//#include "global.h"
//#include "obstacle.h"
///* 障害物定義の雛形のようなもの */
//
//double user_z = 0;
//
//// hogeが生成される時に実行される関数
//static double* hoge_init(void) {
//	double* p;
//	p = malloc(sizeof(double) * 3);
//	p[0] = 15;//敵の座標
//	p[1] = 0;//移動の傾き
//	p[2] = 6.0*rand() / RAND_MAX - 3;//z座標の初期値
//	return p;
//}
//
//// hogeを描画する関数
//static void hoge_render(int birth, double* param) {
//	glPushMatrix();
//
//	glTranslatef(param[0], param[2], 0.0);
//	glDisable(GL_LIGHTING);
//	glEnable(GL_NORMALIZE);
//	glColor3d(0.6, 0.4, 1.0);
//	glScalef(0.7, 0.7, 0.7);
//	if (frame_count % 3 == 1)printf("%6.4lf\t%6.4lf\n", param[0], param[2]);
//	glutSolidOctahedron();
//	glEnable(GL_LIGHTING);
//	glPopMatrix();
//}
//
//// hogeがプレイヤーと衝突したか調べる関数
//static int hoge_judge(int birth, double* param, double z) {
//
//	//あんまりよくない実装だけど…操作してる物体のz座標の持ってき方がわからなかった
//	user_z = z;
//	param[0] -= 0.3 - 0.15*sin(1.0*(frame_count - birth) / 10);
//
//	if (param[0]<0.5 && param[0]>-0.5 && z>param[2] - 0.4 && z<param[2] + 0.4)return 1;
//	return 0;
//}
//
//// hogeを削除する時に実行する関数
//static void hoge_delete(int birth, double* param) {
//
//}
//
//// hogeという障害物の定義を返す関数
//Obstacle getHogeDefinition(void) {
//	// 実際の障害物定義の例
//	Obstacle hoge = {
//		hoge_init,
//		hoge_render,
//		hoge_judge,
//		hoge_delete
//	};
//	return hoge;
//}

//
//#include <stdlib.h>
//#include <GL/glut.h>
//#include "global.h"
//#include "obstacle.h"
//
///* 障害物定義の雛形のようなもの */
//
//double user_z=0;
//
//// hogeが生成される時に実行される関数
//static double* hoge_init(void) {
//	double* p;
//	p = malloc(sizeof(double) * 3);
//	p[0] = 15;//敵の座標
//	p[1] = 0;//移動の傾き
//	p[2] = 6.0*rand()/RAND_MAX-3;//z座標の初期値
//	return p;
//}
//
//// hogeを描画する関数
//static void hoge_render(int birth, double* param) {
//	glPushMatrix();
//
//	if (frame_count - birth < 1.5 * 1000 / refresh_msec && frame_count - birth > 1.5 * 1000 / refresh_msec-2)param[1] = (param[2] - user_z) / param[0];
//
//	//if(frame_count%3==0)printf("%lf\t%lf\n",param[0],param[1]);
//	glTranslatef(param[0], param[2] -param[1]*(frame_count-birth-1. * 1000 / refresh_msec)*0.3, 0.0);
//	glDisable(GL_LIGHTING);
//	glEnable(GL_NORMALIZE);
//	glColor3d(0.0, 1.0, 1.0);
//	glScalef(0.5, 0.5, 0.5);
//
//	glutSolidDodecahedron();
//	glEnable(GL_LIGHTING);
//	glPopMatrix();
//}
//
//// hogeがプレイヤーと衝突したか調べる関数
//static int hoge_judge(int birth, double* param, double z) {
//
//	//あんまりよくない実装だけど…操作してる物体のz座標の持ってき方がわからなかった
//	user_z = z;
//	//phase 1
//	if (frame_count - birth < 0.5 * 1000 / refresh_msec) {
//		param[0] -= 0.3;
//	}
//	//phase 2
//	else if (frame_count - birth < 1.5 * 1000 / refresh_msec) {
//	}
//	//phase 3
//	else {
//		param[0] -= 0.3;
//	}
//
//	if (param[0]<0.5 && param[0]>-0.5 && z>param[2]-0.4 && z<param[2]+0.4)return 1;
//	return 0;
//}
//
//// hogeを削除する時に実行する関数
//static void hoge_delete(int birth, double* param) {
//
//}
//
//// hogeという障害物の定義を返す関数
//Obstacle getHogeDefinition(void) {
//	// 実際の障害物定義の例
//	Obstacle hoge = {
//		hoge_init,
//		hoge_render,
//		hoge_judge,
//		hoge_delete
//	};
//	return hoge;
//}
//
//
////
////#include <stdlib.h>
////#include <GL/glut.h>
////#include "global.h"
////#include "obstacle.h"
////
/////* 障害物定義の雛形のようなもの */
////
////// hogeが生成される時に実行される関数
////static double* hoge_init(void){
////    double* p;
////    p = malloc(sizeof(double)*3);
////    p[0] = 10.0;
////    p[1] = 15.0;
////    p[2] = 20.0;
////    return p;
////}
////
////// hogeを描画する関数
////static void hoge_render(int birth, double* param){
////    glPushMatrix();
////    glTranslatef(15.0-(double)(frame_count-birth)*0.3, 3.0, 0.0);
////    glDisable(GL_LIGHTING);
////    glColor3d(1.0,0.0,0.0);
////    glutSolidCube(1);
////    glEnable(GL_LIGHTING);
////    glPopMatrix();
////}
////
////// hogeがプレイヤーと衝突したか調べる関数
////static int hoge_judge(int birth, double* param, double z){
////    double y = 15.0-(double)(frame_count-birth)*0.3;
////    if(y<0.5 && y>-0.5 && z>2.5 && z<3.5)return 1;
////    return 0;
////}
////
////// hogeを削除する時に実行する関数
////static void hoge_delete(int birth, double* param){
////
////}
////
////// hogeという障害物の定義を返す関数
////Obstacle getHogeDefinition(void){
////    // 実際の障害物定義の例
////    Obstacle hoge = {
////        hoge_init,
////        hoge_render,
////        hoge_judge,
////        hoge_delete
////    };
////    return hoge;
////}
