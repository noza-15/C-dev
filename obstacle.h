
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

typedef struct{
    double* (*init)(double);              // -> パラメータ，プレイヤー位置
    void    (*render)(int,double*,double);     // 誕生秒,パラメータ，プレイヤー位置
    int     (*judge)(int,double*,double);  // 誕生秒,パラメータ,プレイヤー位置 -> 真偽
    void    (*delete)(int,double*,double);     // 誕生秒,パラメータ，プレイヤー位置
} Obstacle;

void refreshResidents(void);
int judgeCollision(double);

void initObstacles(void);
void renderObstacles(void);
void endObstacles(void);


#endif // _OBSTACLE_H_
