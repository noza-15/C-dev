
#ifndef _GAME_H_
#define _GAME_H_

void game_disp(void);
void game_keyboard(unsigned char, int, int);
void over_keyboard(unsigned char, int, int);
void game_special(int, int, int);
void game_mouse(int, int, int, int);
void game_motion(int, int);
void game_passiveMotion(int, int);

#endif // _GAME_H_
