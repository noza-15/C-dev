
#ifndef _SELECTOR_H_
#define _SELECTOR_H_

void selector_disp(void);
void selector_keyboard(unsigned char, int, int);
void selector_special(int, int, int);
void selector_mouse(int, int, int, int);
void selector_motion(int, int);
void selector_passiveMotion(int, int);

#endif // _SELECTOR_H_
