//構造体全てを書いていく

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
    int hp;                  // 現在のHP
    int max_hp;              // 最大HP
    int atk;                 // 攻撃力
    int def;                 // 防御力
    int level;               // レベル
    int exp;                 // 経験値
} Character;

// ==========================================
// 3. 関数の宣言（プロトタイプ宣言）
// ==========================================
// ※ 実装（中身）は story.c や battle.c に書きますが、
//    ここに宣言しておくことで、お互いのファイルをまたいで呼び出せるようになります。

// 【story.c に書く関数】ゲーム全体のループを開始する
void run_game_loop(void);

// 【battle.c に書く関数】戦闘を開始し、勝敗（勝てば1, 負ければ0）を返す
int start_battle(Character *hero, Character *enemy);

// 【A君が今後作成する関数】enemy.txt からステージに応じた敵のデータを読み込む
void load_enemy(int stage, Character *enemy);

#endif // GENERAL_RULES_H