#include <stdio.h>
#include <stdlib.h>
#include "General_Rules.h"

void play_opening_story(Character *hero) {
    system("cls");
    printf("========================================\n");
    printf("         ～ 敵を倒すには運で十分 ～          \n");
    printf("========================================\n\n");
    printf("見知らぬ天井だ...\n");
    printf("頭が割れるように痛い。\n\n");
    printf("> \n\n");
    getchar();

    printf("確かあの時...宝くじで10億円当てて...\n");
    printf("ここは一体。\n\n");
    printf("> \n\n");
    getchar();

    printf("??? : ここは神の世界。\n");
    printf("??? : 今からあなたには魔王を倒す旅に行ってもらいます。\n");
    printf("> \n\n");
    getchar();

    printf("だれだ！！！どこから話しかけている！！\n");
    printf("> \n\n");
    getchar();

    printf("??? : 私はあなたの頭の中に直接話しかけています。\n");
    printf("??? : 時間がありません。手短に...\n\n");
    printf("??? : あなたはこれから、魔王による影響で活気のなくなっている世界に転生してもらいます。\n");
    printf("??? : そこであなたには、その豪運で魔王を討伐してほしいのです。\n");
    printf("> \n\n");
    getchar();

    printf("どういうことだよ!!せっかく10億円当てたから仕事をやめようと...\n\n");
    printf("??? : そう。その豪運があるから選ばれたのです。\n");
    printf("??? : あなたにはあなたにしか使えないユニークスキルを授けます。\n");
    printf("??? : そしてそのスキルで魔王を討伐してほしいのです。\n\n");
    printf("何を言っているんだ！！\n");
    printf("> \n\n");
    getchar();


    printf("...あなたの名前を教えて頂けませんか？\n\n");
    printf("い、いきなり？？！\n\n");
    printf("名前を入力してください >> ");

    // 名前の入力
    scanf("%19s", hero->name);
    while (getchar() != '\n');

    system("cls");

    printf("%sだけど...\n\n", hero->name);
    printf("??? : おお、%s様とおっしゃるのですね！\n", hero->name);
    printf("??? : %s様、どうか世界を救ってください。\n", hero->name);
    printf("??? : では いってらっしゃーーい！！\n\n");
    printf("うわぁぁぁぁぁぁ！！\n");
    printf("> ");
    getchar();
    system("cls");
}

// ==========================================
// 裏ボス出現時の演出メッセージ
// ==========================================
void play_secret_boss_encounter(void) {
    printf("……禍々しい気配を感じる……！\n");
    printf("空間が歪み、何者かがこちらを見ている……\n\n");
}