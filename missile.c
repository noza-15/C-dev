
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
#include "obstacle.h"

// missile����������鎞�Ɏ��s�����֐�
static double* missile_init(double z) {
	double* p;
	p = malloc(sizeof(double) * 8);
	p[0] = 15;//�G�̍��W
	p[1] = 3.14*rand() / RAND_MAX;//�ړ��̌X��
	p[2] = 6.0*rand() / RAND_MAX - 3;//z���W
	p[3] = 0;//x�̔���J�n�n�_(����)
	p[4] = 0;//x�̔���I���n�_(����)
	p[5] = 0;//z�̔���J�n�n�_
	p[6] = 0;//z�̔���I���n�_
	p[7] = p[2];//z���W�̏����l
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

// missile��`�悷��֐�
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

// missile���v���C���[�ƏՓ˂��������ׂ�֐�
static int missile_judge(int birth, double* param, double z) {

	param[3] = param[0]+ 0.4;
	param[4] = param[0]- 0.4;
	param[5] = param[2] - 0.4;
	param[6] = param[2] + 0.4;

	if (param[4]<0 && param[3]>0 && z>param[5] && z<param[6])return 1;
	return 0;
}

// missile���폜���鎞�Ɏ��s����֐�
static void missile_delete(int birth, double* param, double z) {

}

// missile�Ƃ�����Q���̒�`��Ԃ��֐�
Obstacle getMissileDefinition(void) {
	// ���ۂ̏�Q����`�̗�
	Obstacle missile = {
		missile_init,
		missile_render,
		missile_judge,
		missile_delete
	};
	return missile;
}
