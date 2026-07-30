#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "General_Rules.h"

int main(void){
    int start = 0;
    int retry;
    Character hero;

    srand((unsigned int)time(NULL)); // 乱数シードはプログラム起動時に1回だけ初期化

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

            if (scanf("%d", &start) != 1) {
                start = 0; // 不正入力時は無効値としてループを継続させる
            }
            while (getchar() != '\n');
            printf("\n");
        }

        if(start == 1){
            hero.stage = 1;
            hero.hp = 10;
            hero.max_hp = 10;
            hero.atk = 5;
            hero.def = 2;
            hero.mp = 10;
            hero.max_mp = 10;
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

            int read_count = fscanf(fp, "%49s %d %d %d %d %d %d %d %d %d",
                hero.name,
                &hero.stage,
                &hero.hp,
                &hero.max_hp,
                &hero.atk,
                &hero.def,
                &hero.mp,
                &hero.max_mp,
                &hero.level,
                &hero.exp);

            fclose(fp);

            if (read_count != 10) {
                printf("エラー：hero.txtのデータが壊れています。\n");
                return 1;
            }
        }

        system("cls");

//戦闘
        //roop処理で５回
        for (; hero.stage <= 5; hero.stage++) {

            int original_stage = hero.stage; // ループ制御用に退避

            // 裏ボス出現抽選（1%）
            if (rand() % 100 == 0) {
                play_secret_boss_encounter(); // story.cに演出を委譲
                hero.stage = 777;
            }

            int battle_result = start_battle(&hero);

            hero.stage = original_stage; // 抽選に関わらず必ず復元

            if (battle_result == 0) {
                printf("ステージ%dをクリアしました！\n", hero.stage);

                int next_action = 0;
                while (next_action != 1 && next_action != 2) {
                    printf("1 : 次のステージに進む\n");
                    printf("2 : セーブして終了する\n");
                    printf(">>");
                    if (scanf("%d", &next_action) != 1) {
                        next_action = 0;
                    }
                    while (getchar() != '\n');
                }

                if (next_action == 2) {
                    printf("セーブしています...");

                    FILE *fp_save = fopen("hero.txt", "w");
                    if (fp_save != NULL) {
                        // hero.stageは現在クリアした値のまま。次回ロード時はfor文の再開点として
                        // 「hero.stage+1」から始めたいので、保存時点で+1しておく
                        fprintf(fp_save, "%s %d %d %d %d %d %d %d %d %d\n",
                            hero.name,
                            hero.stage + 1,
                            hero.hp,
                            hero.max_hp,
                            hero.atk,
                            hero.def,
                            hero.mp,
                            hero.max_mp,
                            hero.level,
                            hero.exp);

                        fclose(fp_save);
                        printf("データを保存しました！ゲームを終了します。\n");
                        getchar();
                        system("cls");
                        return 0;
                    } else {
                        printf("セーブに失敗しました。\n");
                    }
                }

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
        }

//リトライ確認
        retry = 0;
        while (retry != 1 && retry != 2) {
            printf("1 : Play Again!!\n");
            printf("2 : Quit\n");
            printf(">>");

            if (scanf("%d", &retry) != 1) {
                retry = 0; // 不正入力時は無効値としてループを継続させる
            }
            while (getchar() != '\n');
        }

    }   while(retry == 1);

    return 0;
}