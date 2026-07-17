#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "General_Rules.h"

int main(void){
        // 乱数の初期化
    srand((unsigned int)time(NULL));

        // 題名募集中
    printf("_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n");
    printf("                  TXT RPG               \n");
    printf("_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n");

        // プレイヤーがEnterキーを押すのを待つ処理
    printf("     → enter    \n");
        //Enter取得
    getchar();
        //ターミナルの内容削除
    system("cls");

    return 0;

}