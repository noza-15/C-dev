#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

typedef struct{
    double* (*init)(void);              // -> パラメータ
    void    (*render)(int,double*);     // 誕生秒,パラメータ
    int     (*judge)(int,double*,double);  // 誕生秒,パラメータ,プレイヤー位置 -> 真偽
    void    (*delete)(int,double*);     // 誕生秒,パラメータ
} Obstacle;

void refreshResidents(void);
void renderObstacles(void);
int judgeCollision(double);
void initObstacles(void);


#endif // _OBSTACLE_H_
