#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "General_Rules.h"

int main(void){
    int start = 0;
    int retry;
    Character hero;
    Character enemy;
    

    do{
        start = 0;

        system("cls");

//title
            // 題名募集中
        printf("_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n");
        printf("                  TXT RPG               \n");
        printf("_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n\n\n");

            while(start != 1 && start != 2){
                printf("1 :はじめから    ※前にセーブしていたデータは削除されます\n");
                printf("2 :前のセーブデータから\n");
                printf(">>");

                scanf("%d", &start);
                while (getchar() != '\n');
                printf("\n");
            }

        if(start == 1){ 
                hero.stage = 1;
                hero.mp = 10;
                hero.hp = 10;
                hero.max_hp = 10; 
                hero.atk = 5; 
                hero.def = 2; 
                hero.mp = 10;
                hero.level = 1; 
                hero.exp = 0;

//導入ストーリー
            play_opening_story(&hero);
        }

            else if(start == 2){
                FILE *fp = fopen("hero.txt", "r");

                if (fp == NULL) {
                    printf("エラー：hero.txtが読み込めませんでした。\n");
                    return 1;
                }

                fscanf(fp, "%s %d %d %d %d %d %d %d", 
                    hero.name, 
                    &hero.stage,
                    &hero.hp, 
                    &hero.max_hp, 
                    &hero.atk, 
                    &hero.def, 
                    &hero.level, 
                    &hero.exp);

                    fclose(fp);
                    
            }

        system("cls");
        

//戦闘
            //roop処理で５回
        for (; hero.stage <= 5; hero.stage++) {
    
                // 戦闘開始（ステージ番号と勇者を渡す）
            int battle_result = start_battle(&hero);
            

            if (battle_result == 0) {
                printf("ステージ%dに進みます...\n", hero.stage + 1);

                printf("\n");
                printf("?");
                getchar();
                system("cls");
            }
                
                else if (battle_result == 1) {
                    printf("ゲームオーバー！\n");
                    printf("もう一度異世界転生する？\n");
                    break;
                } 

                        else if (battle_result==2){
                            system("cls");

                            printf("おめでとう！！この世界の平和は保たれた！！！\n");
                            printf("君は最強の戦士だ！\n\n");

                            printf("もう一度最初から遊ぶ！！\n");

                            printf("?");

                            printf("\n");
                            getchar();
                            
                            break;
                        }

                            else if (battle_result==3){
                                printf("セーブしています...");

                                FILE *fp_save = fopen("hero.txt", "w");

                                if(fp_save != NULL){
                                    fprintf(fp_save, "%s %d %d %d %d %d %d %d\n",
                                    hero.name,
                                    hero.stage,
                                    hero.hp,
                                    hero.max_hp,
                                    hero.atk,
                                    hero.def,
                                    hero.level,
                                    hero.exp);

                                    fclose(fp_save);
                                    printf("データを保存しました！ゲームを終了します。\n");

                                    getchar();
                                    system("cls");

                                    return 0;
                                    }
                               
                                         else {
                                                printf("セーブに失敗しました。\n");
                                                break;
                                            }
                                }

        }


//リトライ確認
        printf("1 : Play Again!!\n");
        printf("2 : Quit\n");

        scanf("%d", &retry);
        while(getchar() != '\n');

    }   while(retry == 1);
}