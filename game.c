
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include "global.h"
#include "game.h"
#include "player.h"
#include "obstacle.h"
#include "background.h"
#include "score.h"

#include <stdio.h> // debug

#ifndef M_PI
#define M_PI 3.14159265
#endif // M_PI

#ifndef M_PI_2
#define M_PI_2 1.57079633
#endif // M_PI_2

static int GAME_INIT_FLAG = UNFINISHED; // game state
static int DEVELOPPE_MODE = OFF;        // game state

static double camera_x;             // devlopper camera
static double camera_y;             // devlopper camera
static double camera_z;             // devlopper camera
static double camera_px;            // devlopper camera
static double camera_py;            // devlopper camera
static double camera_pz;            // devlopper camera
static double ang_x;
static double ang_y;
static int motion_x;                // developper camera
static int motion_y;                // developper camera

static int game_over_flag;          // game system

static int score;
static int game_start_frame = 0;

static void updateDevCam(void) {
    const double speed = 0.00007;
    ang_x += motion_x * speed;
    ang_y -= motion_y * speed;
    if (ang_x < -M_PI) ang_x += M_PI * 2;
    else if (ang_x > M_PI) ang_x -= M_PI * 2;
    if (ang_y < -M_PI_2) ang_y = -M_PI_2;
    else if (ang_y > M_PI_2) ang_y = M_PI_2;
    camera_px = sin(ang_x) * cos(ang_y);
    camera_py = sin(ang_y);
    camera_pz = cos(ang_x) * cos(ang_y);
}

// 各フレームごとにゲームの状態を更新する関数
static void game_refresh(void) {
    refreshResidents();
    double z = getPlayerPosition();
    if (judgeCollision(z))game_over_flag = ON;
}

// 画面に文字列を表示する関数
static void printString(char *str, int x0, int y0) {
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewport_width, viewport_height, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    int length = 0;
    while (str[length] != '\0')length++;
    int i;
    for (i = 0; i < length; i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ゲーム開始時に実行する
static void game_init(void) {
    glEnable(GL_LIGHTING);
    initPlayer();
    initObstacles();
    initBackground();

    camera_x = 0.0;
    camera_y = 0.0;
    camera_z = 20.0;
    camera_px = 0.0;
    camera_py = 0.0;
    camera_pz = 1.0;

    game_over_flag = OFF;
    DEVELOPPE_MODE = OFF;
    printf("game init\n");
}

// ゲームを終わる時に実行する
static void game_exit(void) {
    printf("now game exit.\n");
    endPlayer();
    endObstacles();
    endBackground();
}


// debug ///////////////////////////////////////////////////////////////////////

// 座標軸を描画する関数
static void coordinate_axis(double length) {
    glDisable(GL_LIGHTING);
    glPushMatrix();

    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3d(length, 0.0, 0.0);
    glVertex3d(-length, 0.0, 0.0);
    glEnd();

    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3d(0.0, length, 0.0);
    glVertex3d(0.0, -length, 0.0);
    glEnd();

    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, length);
    glVertex3d(0.0, 0.0, -length);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// 座標軸方向を表示する
static void direction(double position, double scale) {
    glDisable(GL_LIGHTING);
    glPushMatrix();

    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glTranslatef(position, 0.0, 0.0);
    glRotated(90, 0.0, 1.0, 0.0);
    glutSolidCone(0.1 * scale, 0.5 * scale, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.0, 1.0, 0.0);
    glTranslatef(0.0, position, 0.0);
    glRotated(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.1 * scale, 0.5 * scale, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, position);
    glutSolidCone(0.1 * scale, 0.5 * scale, 10, 10);
    glPopMatrix();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// XZ平面上に目盛りを追加する関数
static void zx_lines(int max, double interval) {
    glDisable(GL_LIGHTING);
    glPushMatrix();

    int i;
    double length = max * interval;

    glColor3d(0.2, 0.2, 0.2);
    glBegin(GL_LINES);
    for (i = 1; i < 20; i++) {
        glVertex3d(i * interval, 0, length);
        glVertex3d(i * interval, 0, -length);

        glVertex3d(-i * interval, 0, length);
        glVertex3d(-i * interval, 0, -length);

        glVertex3d(length, 0, i * interval);
        glVertex3d(-length, 0, i * interval);

        glVertex3d(length, 0, -i * interval);
        glVertex3d(-length, 0, -i * interval);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// XY平面上に目盛りを追加する関数
static void xy_lines(int max, double interval) {
    glDisable(GL_LIGHTING);
    glPushMatrix();

    int i;
    double length = max * interval;

    glColor3d(0.2, 0.2, 0.2);
    glBegin(GL_LINES);
    for (i = 1; i < 20; i++) {
        glVertex3d(i * interval, length, 0);
        glVertex3d(i * interval, -length, 0);

        glVertex3d(-i * interval, length, 0);
        glVertex3d(-i * interval, -length, 0);

        glVertex3d(length, i * interval, 0);
        glVertex3d(-length, i * interval, 0);

        glVertex3d(length, -i * interval, 0);
        glVertex3d(-length, -i * interval, 0);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

////////////////////////////////////////////////////////////////////////////////

// プレイヤーのカメラ変換行列をかける関数()
static void setPlayerCam(void) {
    glMatrixMode(GL_MODELVIEW);     // モデルビュー行列モードにする
    glLoadIdentity();               // 単位行列をロード
	if (getWave() > 12) {
		gluLookAt(20 * sin((double)(frame_count-game_start_frame-200*12) / 300),0.0,20 * cos((double)(frame_count-game_start_frame- 200 * 12) / 300), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else {
		gluLookAt(0, 0.0, 20, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
}

// 開発者用のカメラ変換行列をかける関数
static void setDevCam(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_x, camera_y, camera_z,
              camera_x + camera_px, camera_y + camera_py, camera_z - camera_pz, 0.0, 1.0, 0.0);
}

// 画面サイズに合わせてカメラのアスペクト比を決める
static void adjust_aspect(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double) width / (double) height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// ワールドの全てを描画する関数
static void renderAll(int width, int height) {

    adjust_aspect(width, height);
    renderPlayer();
    renderObstacles();
    renderBackground();
	renderScore();
}

/////////////////////////////////////////////////////////////////////////////////
#ifndef _MSC_VER

//スコアの表示
void renderScore(){
    score += getWave();
    char buf[100];
    glColor3d(1, 0, 0);
    sprintf(buf,"score %d", score);
    printString(buf, window_width - 120, 20);

    if ((frame_count - game_start_frame) % 200 == 0)changeWave();
    sprintf(buf, "WAVE: %d", getWave());
    printString(buf,
        window_width/2 +rand() % (1+getWave()*getWaveChangedFlag()),
        window_height/2+40+rand()%(1+ getWave() * getWaveChangedFlag()));

    int spawn_time  = (int)(100.0*pow(0.9, getWave()));
    int remain      = frame_count % spawn_time;

    sprintf(buf, "respawn:");
    for(int i = 0; i < spawn_time; i++){
        if(i > remain)  strcat(buf, "_");
        else            strcat(buf, "=");
    }
    printString(buf,10,window_height-10);
}

// ゲームオーバー画面の描画
void over_disp(void) {
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.0, 0.0);
    printString("GAME OVER...", 30, 30);
    printString("Press c key to continue.", 30, 60);
    printString("Press r key to restart.", 30, 80);
    printString("Press b key to go back to menu.", 30, 100);
    char buf[64];
    sprintf(buf, "score %d", score);
    printString(buf,
        window_width / 2 + rand() % (1 + 3 * getWaveChangedFlag()),
        window_height / 2 + 40 + rand() % (1 + 3 * getWaveChangedFlag()));
    glEnable(GL_LIGHTING);
    glViewport(viewport_start_x, viewport_start_y, viewport_width, viewport_height);
    glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////////////
#else

//スコアの表示
void renderScore() {
	score  +=getWave();
	char buf[100];
	glColor3d(1, 0, 0);
	sprintf_s(buf, sizeof(buf),"score %d", score);
	printString(buf, window_width - 120, 20);

	if ((frame_count - game_start_frame) % 200 == 0)changeWave();
	sprintf_s(buf, sizeof(buf), "WAVE: %d", getWave());
	printString(buf,
		window_width/2 +rand() % (1+getWave()*getWaveChangedFlag()),
		window_height/2+40+rand()%(1+ getWave() * getWaveChangedFlag()));

	int spawn_time = (int)(100.0*pow(0.9, getWave()));
	int remain =  frame_count % spawn_time;

	sprintf_s(buf, sizeof(buf), "respawn:");
	for (int i = 0; i < spawn_time; i++)
	{
		if (i > remain) {
			strcat_s(buf, sizeof(buf), "_");
		}
		else{
			strcat_s(buf, sizeof(buf), "=");
		}
	}

	printString(buf,10,window_height-10);

}

// ゲームオーバー画面の描画
void over_disp(void) {
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.0, 0.0);
    printString("GAME OVER...", 30, 30);
    printString("Press c key to continue.", 30, 60);
    printString("Press r key to restart.", 30, 80);
    printString("Press b key to go back to menu.", 30, 100);
	char buf[64];
	sprintf_s(buf, sizeof(buf), "score %d", score);
	printString(buf,
		window_width / 2 + rand() % (1 + 3 * getWaveChangedFlag()),
		window_height / 2 + 40 + rand() % (1 + 3 * getWaveChangedFlag()));
    glEnable(GL_LIGHTING);
    glViewport(viewport_start_x, viewport_start_y, viewport_width, viewport_height);
    glutSwapBuffers();
}

#endif // __STDC_ANALYZABLE__
//////////////////////////////////////////////////////////////////////////////////

//スコア計算のリセット
static void resetScore() {
	game_start_frame = frame_count;
	score = 0;
	resetWave();
}

// ゲーム状態で呼び出される描画関数
void game_disp(void) {

    if (GAME_INIT_FLAG == UNFINISHED) {
        game_init();
        GAME_INIT_FLAG = FINISHED;
    }
    // ゲームの内部状態を更新
    game_refresh();
    // 開発者モードなら
    if (DEVELOPPE_MODE == ON && game_over_flag == OFF) {
        glDisable(GL_LIGHTING);
        glColor3d(0.9, 0.3, 0.3);
        printString("Press x key to exit developer mode", 30, 30);
        glEnable(GL_LIGHTING);

        // 左半分に描画するよう設定
        glViewport(viewport_start_x, viewport_start_y + viewport_height / 2,
                   viewport_width / 2, viewport_height / 2);
        setPlayerCam();
        renderAll(viewport_width / 2, viewport_height / 2);

        // 右上に軸付きのプレイ画面を表示
        glViewport(viewport_start_x, viewport_start_y,
                   viewport_width / 2, viewport_height / 2);
        adjust_aspect(viewport_width / 2, viewport_height / 2);
        setPlayerCam();
        coordinate_axis(20.0);  // 座標軸を表示
        xy_lines(20, 0.5);
        renderAll(viewport_width / 2, viewport_height / 2);

        // 右下に描画するように設定
        glViewport(viewport_start_x + viewport_width / 2, viewport_start_y,
                   viewport_width / 2, viewport_height);
        adjust_aspect(viewport_width / 2, viewport_height);
        updateDevCam();
        setDevCam();
        coordinate_axis(20.0);  // 座標軸を表示
        zx_lines(20, 0.5);          // XZ平面の目盛りを表示
        direction(4.0, 1.0);     // 座標軸の方向を表示
        renderAll(viewport_width / 2, viewport_height);

    } else if (game_over_flag == OFF) {                          // 開発者モードでないなら
        glDisable(GL_LIGHTING);
        glColor3d(0.9, 0.3, 0.3);
        printString("Press x key to developer mode", 30, 30);
        printString("Press UP key to start and jump", 30, 60);
        glEnable(GL_LIGHTING);



        glViewport(viewport_start_x, viewport_start_y,
                   viewport_width, viewport_height);
        setPlayerCam();
        renderAll(viewport_width, viewport_height);    // プレイヤー向けの描画関数を呼び出す
    } else {
        DEVELOPPE_MODE = OFF;
        state = GAME_OVER;
    }

    glutSwapBuffers();              // 描画を更新
}

// キーボードの入力で呼び出される関数
void game_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'm':
            game_exit();
            if (DEVELOPPE_MODE == ON) {
                printf("developer mode off.\n");
                DEVELOPPE_MODE = OFF;
            }
            GAME_INIT_FLAG = UNFINISHED;
            srand(1);
            state = MENU;
            break;
        case ' ':
            printf("push space\n");
            jump();
            break;
        case 'e':
//            state = GAME_OVER;
            break;
        case 'o':
            if (DEVELOPPE_MODE == ON) state = GAME_OVER;
            break;
        case 'x':
            printf("push x\n");
            if (DEVELOPPE_MODE == ON) {
                printf("developer mode off.\n");
                glutSetCursor(GLUT_CURSOR_INHERIT);
                DEVELOPPE_MODE = OFF;
            } else {
                printf("developer mode on.\n");
                glutSetCursor(GLUT_CURSOR_NONE);
                ang_x = 0.0;
                ang_y = 0.0;
                motion_x = 0;
                motion_y = 0;
                DEVELOPPE_MODE = ON;
                glutWarpPointer(viewport_start_x + 3 * viewport_width / 4, viewport_start_y + viewport_height / 2);
            }
    }
}

void over_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'b':
            printf("I want to go back to menu!\n");
            game_exit();
            GAME_INIT_FLAG = UNFINISHED;
            srand(1);
            state = MENU;
            break;
        case 'c':
            printf("I want to continue!\n");
            game_over_flag = OFF;
            initPlayer();
            state = GAME;
            break;
        case 'r':
            printf("I want to restart!\n");
            game_exit();
            GAME_INIT_FLAG = UNFINISHED;
            srand(1);
			resetScore();
            state = SELECTOR;
            break;
    }
}

// 特殊なキーの入力で呼び出される関数
void game_special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            jump();
            break;
        case GLUT_KEY_LEFT:
//            game_exit();
            if (DEVELOPPE_MODE == ON) {
                printf("developer mode off.\n");
                DEVELOPPE_MODE = OFF;
            }
//            GAME_INIT_FLAG = UNFINISHED;
//            state = MENU;
            break;
    }
}

// マウスの入力で呼び出される関数
void game_mouse(int button, int mouse_state, int x, int y) {
}

// マウスドラッグの入力で呼び出される関数
void game_motion(int x, int y) {
}

// マウスの動きの入力で呼び出される関数
void game_passiveMotion(int x, int y) {
    static int wrap = 0;
    if (DEVELOPPE_MODE == ON) {
        if (!wrap) {
            motion_x = x - viewport_start_x - 3 * viewport_width / 4;
            motion_y = y - viewport_start_y - viewport_height / 2;
            wrap = 1;
            glutWarpPointer(viewport_start_x + 3 * viewport_width / 4, viewport_start_y + viewport_height / 2);
        } else {
            wrap = 0;
        }
    }
}

void over_passiveMotion(int x, int y) {
}
