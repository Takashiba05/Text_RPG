#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "General_Rules.h"

#define SKILL_FILE "skills.txt"
#define MAX_SKILL_POOL 200
#define BASIC_ATTACK_ID 1 // パンチ（FIXED, MP消費0）。敵のMP不足時の共通フォールバックとして使う
#define EXP_PER_LEVEL_BASE 100 // 次のレベルに必要な累積EXP = hero->level * EXP_PER_LEVEL_BASE
#define GROWTH_HP(lv)  (8 + (lv) * 2)   // Lv2:+12 → Lv10:+28 → Lv14:+36
#define GROWTH_ATK(lv) (1 + (lv) / 2)   // Lv2:+2  → Lv10:+6  → Lv14:+8
#define GROWTH_DEF(lv) (1 + (lv) / 4)   // Lv2:+1  → Lv10:+3  → Lv14:+4
#define GROWTH_MP(lv)  (1 + (lv) / 3)   // Lv2:+1  → Lv10:+4  → Lv14:+5

// ==========================================
// skills.txt を全件読み込む
// ==========================================
static int load_all_skills(Skill *pool) {
    FILE *fp = fopen(SKILL_FILE, "r");
    if (fp == NULL) {
        printf("エラー：%s が読み込めませんでした。\n", SKILL_FILE);
        return 0;
    }

    int count = 0;
    while (count < MAX_SKILL_POOL &&
           fscanf(fp, "%d %29s %7s %d %d %d",
                  &pool[count].id,
                  pool[count].name,
                  pool[count].pool,
                  &pool[count].power,
                  &pool[count].mp_cost,
                  &pool[count].stage) == 6) {
        count++;
    }

    fclose(fp);
    return count;
}

// ==========================================
// 指定プール(タグ)からランダムに1件選ぶ（重複を避けるため used 配列を見る）
// ==========================================
static int pick_random_skill(Skill *pool, int count, const char *tag, int stage,
                              int *skill_id_out, int *used, int used_count) {
    int candidates[MAX_SKILL_POOL];
    int cand_count = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(pool[i].pool, tag) == 0 && pool[i].stage == stage) {
            int already = 0;
            for (int j = 0; j < used_count; j++) {
                if (used[j] == pool[i].id) { already = 1; break; }
            }
            if (!already) {
                candidates[cand_count++] = i;
            }
        }
    }

    if (cand_count == 0) return 0; // 該当技なし

    int chosen = candidates[rand() % cand_count];
    *skill_id_out = pool[chosen].id;
    return 1;
}

// ==========================================
// 勇者の技構成：固定2つ(パンチ, MP回復) + 弱2つ + 強2つ
// ==========================================
static void assign_hero_skills(Character *hero, Skill *pool, int count) {
    hero->skill_count = 0;
    int used[MAX_SKILL_SLOT] = {0};

    for (int i = 0; i < count && hero->skill_count < MAX_SKILL_SLOT; i++) {
        if (strcmp(pool[i].pool, "FIXED") == 0 && pool[i].stage == 0) {
            hero->skill_id[hero->skill_count] = pool[i].id;
            used[hero->skill_count] = pool[i].id;
            hero->skill_count++;
        }
    }

    for (int n = 0; n < 2; n++) {
        int id;
        if (pick_random_skill(pool, count, "WEAK", 0, &id, used, hero->skill_count)) {
            hero->skill_id[hero->skill_count] = id;
            used[hero->skill_count] = id;
            hero->skill_count++;
        }
    }

    for (int n = 0; n < 2; n++) {
        int id;
        if (pick_random_skill(pool, count, "STRONG", 0, &id, used, hero->skill_count)) {
            hero->skill_id[hero->skill_count] = id;
            used[hero->skill_count] = id;
            hero->skill_count++;
        }
    }
}

// ==========================================
// 敵の技構成：現在ステージの ENEMY タグから4つランダム
// ==========================================
static void assign_enemy_skills(Character *enemy, Skill *pool, int count, int stage) {
    enemy->skill_count = 0;
    int used[MAX_SKILL_SLOT] = {0};

    for (int n = 0; n < 4; n++) {
        int id;
        if (pick_random_skill(pool, count, "ENEMY", stage, &id, used, enemy->skill_count)) {
            enemy->skill_id[enemy->skill_count] = id;
            used[enemy->skill_count] = id;
            enemy->skill_count++;
        }
    }

    // 該当ステージのENEMY技が1件も見つからなかった場合の防御(0除算防止)
    if (enemy->skill_count == 0) {
        fprintf(stderr, "警告: stage=%d に該当するENEMY技が見つかりません\n", stage);
    }
}

// ==========================================
// 技IDからSkillデータを引く
// ==========================================
static Skill *find_skill(Skill *pool, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (pool[i].id == id) return &pool[i];
    }
    return NULL;
}

// ==========================================
// 技を1つ選ばせて実行（勇者=手入力、敵=ランダム）
// ==========================================
static void use_skill(Character *attacker, Character *defender, Skill *used_skill) {
    if (used_skill->mp_cost < 0) {
        attacker->mp -= used_skill->mp_cost;
        if (attacker->mp > attacker->max_mp) attacker->mp = attacker->max_mp;
        printf("%s は %s を使った！ MPが回復した！\n", attacker->name, used_skill->name);
    } else {
        int damage = (attacker->atk + used_skill->power) - defender->def;
        if (damage < 1) damage = 1;
        defender->hp -= damage;
        if (defender->hp < 0) defender->hp = 0;
        attacker->mp -= used_skill->mp_cost;
        printf("%s は %s を使った！ %s に %d ダメージ！\n",
               attacker->name, used_skill->name, defender->name, damage);
    }
}

static void gain_exp_and_level_up(Character *hero, int exp_gained) {
    hero->exp += exp_gained;
    printf("\n%s は %d の経験値を獲得した！\n", hero->name, exp_gained);

    int leveled_up = 0;
    while (hero->exp >= hero->level * EXP_PER_LEVEL_BASE) {
        hero->exp -= hero->level * EXP_PER_LEVEL_BASE;
        hero->level++;
        hero->max_hp += GROWTH_HP(hero->level);
        hero->atk    += GROWTH_ATK(hero->level);
        hero->def    += GROWTH_DEF(hero->level);
        hero->max_mp += GROWTH_MP(hero->level);
        leveled_up = 1;
    }

    if (leveled_up) {
        hero->hp = hero->max_hp; // レベルアップ時はHP・MPを全回復
        hero->mp = hero->max_mp;
        printf("レベルアップしました！！ %s は Lv.%d になった！\n", hero->name, hero->level);
        printf("(HP上限:%d 攻撃力:%d 防御力:%d MP上限:%d)\n", hero->max_hp, hero->atk, hero->def, hero->max_mp);
        printf("体力とMPが全回復した！\n");
    } else {
        printf("(次のレベルまであとEXP %d)\n", hero->level * EXP_PER_LEVEL_BASE - hero->exp);
    }
}

// ==========================================
// メイン：戦闘処理
// 戻り値: 0=勝利/1=敗北/2=魔王撃破
// ==========================================
int start_battle(Character *hero) {
    // srand はここでは呼ばない（main.c側で起動時に1回だけ実行する方針に統一）

    Skill pool[MAX_SKILL_POOL];
    int skill_count = load_all_skills(pool);
    if (skill_count == 0) {
        printf("エラー：技データの読み込みに失敗したため、戦闘を開始できません。\n");
        return 1;
    }

    Character enemy;
    load_enemy(hero->stage, &enemy);

    assign_hero_skills(hero, pool, skill_count);
    assign_enemy_skills(&enemy, pool, skill_count, hero->stage);

    printf("%s が現れた！\n", enemy.name);
    printf("Lv.%-3d %s  HP:%d/%d\n", enemy.level, enemy.name, enemy.hp, enemy.max_hp);

    printf("➤ ");
    getchar();

    while (hero->hp > 0 && enemy.hp > 0) {
        printf("\n--- %s のターン ---\n", hero->name);
        printf("Lv.%-3d %s  HP:%d/%d MP:%d/%d\n", hero->level, hero->name, hero->hp, hero->max_hp, hero->mp, hero->max_mp);
        printf("Lv.%-3d %s  HP:%d/%d MP:%d/%d\n", enemy.level, enemy.name, enemy.hp, enemy.max_hp, enemy.mp, enemy.max_mp);
        printf("\n");
        
        for (int i = 0; i < hero->skill_count; i++) {
            Skill *s = find_skill(pool, skill_count, hero->skill_id[i]);
            printf("%d: %s (MP%d)\n", i + 1, s->name, s->mp_cost);
        }
        printf("\n");
        printf("使う技を選んでください >> ");

        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice < 1 || choice > hero->skill_count) {
            printf("その技は選べません。パンチを使います。\n");
            choice = 1;
        }

        Skill *chosen = find_skill(pool, skill_count, hero->skill_id[choice - 1]);
        if (chosen->mp_cost > 0 && hero->mp < chosen->mp_cost) {
            printf("MPが足りない！パンチを使います。\n");
            chosen = find_skill(pool, skill_count, hero->skill_id[0]);
        }
        use_skill(hero, &enemy, chosen);

        if (enemy.hp <= 0) break;

        printf("➤ ");
        getchar();

        printf("\n--- %s のターン ---\n", enemy.name);
        printf("HP:%d/%d MP:%d/%d\n", enemy.hp, enemy.max_hp, enemy.mp, enemy.max_mp);

        printf("➤ ");
        getchar();

        // MPが足りている技だけを候補として絞り込む
        int usable_skills[MAX_SKILL_SLOT];
        int usable_count = 0;
        for (int i = 0; i < enemy.skill_count; i++) {
            Skill *s = find_skill(pool, skill_count, enemy.skill_id[i]);
            if (s != NULL && s->mp_cost <= enemy.mp) {
                usable_skills[usable_count++] = enemy.skill_id[i];
            }
        }

        Skill *enemy_skill;
        if (usable_count > 0) {
            int enemy_skill_id = usable_skills[rand() % usable_count];
            enemy_skill = find_skill(pool, skill_count, enemy_skill_id);
        } else {
            printf("%s はMPが足りず、パンチを繰り出した！\n", enemy.name);
            enemy_skill = find_skill(pool, skill_count, BASIC_ATTACK_ID);
        }

        if (enemy_skill != NULL) {
            use_skill(&enemy, hero, enemy_skill);
        }
    }

    if (hero->hp <= 0) {
        return 1;
    }
    
    // 勝利：EXP加算・レベルアップ処理
    gain_exp_and_level_up(hero, enemy.exp);
    printf("\n");
    printf("続けるには何かキーを押してください...");
    getchar();
    
    if (enemy.is_boss) {
        return 2;
    }
    return 0;
}