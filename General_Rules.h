#ifndef GENERAL_RULES_H
#define GENERAL_RULES_H

// ==========================================
// 1. 定数（ゲーム全体で使う設定値）の定義
// ==========================================
#define NAME_MAX_LEN 50  // 名前の最大文字数

// ==========================================
// 2. 構造体（キャラクターのデータ構造）の定義
// ==========================================
typedef struct {
    char name[NAME_MAX_LEN]; // キャラクターの名前
    int stage;               // 現在のステージ
    int hp;                  // 現在のHP
    int max_hp;              // 最大HP
    int atk;                 // 攻撃力
    int def;                 // 防御力
    int mp;                  // 残りMP
    int level;               // レベル
    int exp;                 // 経験値
    
} Character;

// ==========================================
// 3. 関数の宣言（プロトタイプ宣言）
// ==========================================
// 【story.c に書く関数】ゲーム全体のループを開始する
void play_opening_story(Character *hero);

// 【battle.c に書く関数
int start_battle(Character *hero);

//enemy.txt からステージに応じた敵のデータを読み込む
void load_enemy(Character *enemy);

#endif // GENERAL_RULES_H