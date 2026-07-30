#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "General_Rules.h"

#define ENEMY_FILE "enemy.txt"
#define MAX_ENEMY_POOL 30

// ==========================================
// enemy.txt を全件読み込む
// ==========================================
static int load_all_enemies(Character *pool) {
    FILE *fp = fopen(ENEMY_FILE, "r");
    if (fp == NULL) {
        printf("エラー：%s が読み込めませんでした。\n", ENEMY_FILE);
        return 0;
    }

    int count = 0;
    while (count < MAX_ENEMY_POOL &&
           fscanf(fp, "%49s %d %d %d %d %d %d %d %d",
                  pool[count].name,
                  &pool[count].stage,
                  &pool[count].hp,
                  &pool[count].max_hp,
                  &pool[count].atk,
                  &pool[count].def,
                  &pool[count].mp,
                  &pool[count].level,
                  &pool[count].exp) == 9) {
        count++;
    }

    fclose(fp);
    return count;
}

// ==========================================
// enemy.txt からステージに応じた敵データを読み込み、
// is_boss（魔王かどうか）もここでセットする
// ==========================================
void load_enemy(int stage, Character *enemy) {
    Character pool[MAX_ENEMY_POOL];
    int count = load_all_enemies(pool);

    if (count == 0) {
        strcpy(enemy->name, "謎の敵");
        enemy->stage = stage;
        enemy->hp = enemy->max_hp = 10;
        enemy->atk = 5;
        enemy->def = 2;
        enemy->mp = enemy->max_mp = 0;
        enemy->level = 1;
        enemy->exp = 0;
        enemy->is_boss = 0;
        return;
    }

    // 指定ステージに該当する敵の候補を集める
    int candidates[MAX_ENEMY_POOL];
    int cand_count = 0;
    for (int i = 0; i < count; i++) {
        if (pool[i].stage == stage) {
            candidates[cand_count++] = i;
        }
    }

    if (cand_count == 0) {
        strcpy(enemy->name, "謎の敵");
        enemy->stage = stage;
        enemy->hp = enemy->max_hp = 10;
        enemy->atk = 5;
        enemy->def = 2;
        enemy->mp = enemy->max_mp = 0;
        enemy->level = 1;
        enemy->exp = 0;
        enemy->is_boss = 0;
        return;
    }

    // 候補からランダムに1体選ぶ
    int chosen = candidates[rand() % cand_count];

    strcpy(enemy->name, pool[chosen].name);
    enemy->stage = pool[chosen].stage;
    enemy->hp = pool[chosen].hp;
    enemy->max_hp = pool[chosen].max_hp;
    enemy->atk = pool[chosen].atk;
    enemy->def = pool[chosen].def;
    enemy->mp = pool[chosen].mp;
    enemy->max_mp = 0; // 敵はmax_mpを使わない
    enemy->level = pool[chosen].level;
    enemy->exp = pool[chosen].exp;

    // stage5(通常ラスボス群)と777(裏ボス)は全員is_boss扱い
    if (enemy->stage == 5 || enemy->stage == 777) {
        enemy->is_boss = 1;
    } else {
        enemy->is_boss = 0;
    }
}