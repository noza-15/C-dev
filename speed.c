#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"
/* ��Q����`�̐��`�̂悤�Ȃ��� */

//double user_z = 0;

// speed����������鎞�Ɏ��s�����֐�
static double* speed_init(void) {
	double* p;
	p = malloc(sizeof(double) * 3);
	p[0] = 15;//�G�̍��W
	p[1] = 0;//�ړ��̌X��
	p[2] = 6.0*rand() / RAND_MAX - 3;//z���W�̏����l
	return p;
}

// speed��`�悷��֐�
static void speed_render(int birth, double* param) {

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

// speed���v���C���[�ƏՓ˂��������ׂ�֐�
static int speed_judge(int birth, double* param, double z) {
	if (param[0]<0.5 && param[0]>-0.5 && z>param[2] - 0.4 && z<param[2] + 0.4)return 1;
	return 0;
}

// speed���폜���鎞�Ɏ��s����֐�
static void speed_delete(int birth, double* param) {

}

// speed�Ƃ�����Q���̒�`��Ԃ��֐�
Obstacle getSpeedDefinition(void) {
	//���ۂ̏�Q����`�̗�
	Obstacle speed = {
		speed_init,
		speed_render,
		speed_judge,
		speed_delete
	};
	return speed;
}
