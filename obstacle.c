
#include <stdlib.h>
#include "global.h"
#include "obstacle.h"

#include "hoge.h"

#include <stdio.h> // debug

#define OBSTACLE_NUMBER 4   // 全ての障害物の種類の数

// 障害物定義リスト
static Obstacle *ObstacleDefenition;

// 実際に存在するものの定義
typedef struct{
    int key;
    double* param;
    int birth;
}resident;

static resident *residentList;     // 存在する障害物のリスト
static int residentList_start;
static int residentList_end;

#define MAX_RESIDENT 100    // 存在して良い障害物の上限

// 確率的に障害物を発生して返す関数
static resident residentBirth(void){
    //int i = rand();
    int i = 0;
    resident x;
    x.key = i;
    x.param = ObstacleDefenition[i].init();
    x.birth = frame_count;
    return x;
}

// 各フレームごとに実行する
// 確率や条件的に障害物を発生させるかさせないかを決めて障害物リストを更新する関数
// 消したり生成したり
void refreshResidents(void){
    if(frame_count%70==0){
        residentList[residentList_end++] = residentBirth();
    }
    int j;
    for(j=residentList_start;j<residentList_end;j++){
        if(residentList[j].birth < frame_count - 700){
            residentList_start = j+1;
        };
    }
}

// 全ての障害物と衝突していないか判定する関数
int judgeCollision(double z){
    int i;
    for(i=residentList_start;i<residentList_end;i++){
        resident obj = residentList[i];
        if(ObstacleDefenition[obj.key].judge(obj.birth,obj.param,z)){
            return 1;
        }
    }
    return 0;
}

// 障害物の初期化
void initObstacles(void){
    ObstacleDefenition = malloc(sizeof(Obstacle)*OBSTACLE_NUMBER);
    int i;
    for(i=0;i<OBSTACLE_NUMBER;i++){
        ObstacleDefenition[i] = getHogeDefinition();
    }
    residentList = malloc(sizeof(resident)*100);
    residentList_start = 0;
    residentList_end = 0;
}

// 全ての障害物を描画する
void renderObstacles(void){
    //printf("call render obstacles\n");
    int i;
    for(i=residentList_start;i<residentList_end;i++){
        resident obj = residentList[i];
        ObstacleDefenition[obj.key].render(obj.birth,obj.param);
    }
}

// 障害物の終了
void endObstacles(void){
    free(residentList);
}
