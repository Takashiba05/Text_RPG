#ifndef GENERAL_RULES_H
#define GENERAL_RULES_H

// ==========================================
// 1. 定数（ゲーム全体で使う設定値）の定義
// ==========================================
#define NAME_MAX_LEN 50   // 名前の最大文字数
#define MAX_SKILL_SLOT 6  // キャラクターが同時に持てる技の最大数
                           // (勇者：FIXED2 + WEAK2 + STRONG2 = 6、敵：ENEMY4 なので6あれば足りる)

// ==========================================
// 2. 構造体（キャラクターのデータ構造）の定義
// ==========================================
typedef struct {
    char name[NAME_MAX_LEN]; // キャラクターの名前

    int stage;                // 現在のステージ
    int hp;                   // 現在のHP
    int max_hp;                // 最大HP
    int atk;                   // 攻撃力
    int def;                   // 防御力
    int mp;                    // 現在のMP
    int max_mp;                 // 最大MP（MP回復技の上限チェックに使用）
    int level;                  // レベル
    int exp;                    // 経験値（hero：自身の経験値／enemy：倒した時の獲得経験値）
    int skill_id[MAX_SKILL_SLOT]; // 現在習得している技のID一覧
    int skill_count;               // 習得している技の数
    int is_boss;                    // 魔王（最終ボス）かどうか（0=通常, 1=魔王, 裏ボスも1）
} Character;

// 技（スキル）のデータ構造
typedef struct {
    int id;              // 技の一意なID
    char name[20];         // 技の名前（fscanf "%19s" に対応）
    char pool[8];            // 技の分類タグ（FIXED / WEAK / STRONG / ENEMY）
    int power;                // 技の威力（攻撃力に加算される）
    int mp_cost;                // 消費MP（マイナスの場合はMP回復技を意味する）
    int stage;                   // 使用可能なステージ（ENEMYタグの場合のみ意味を持つ）
} Skill;

// ==========================================
// 3. 関数の宣言（プロトタイプ宣言）
// ==========================================

// 【story.c に書く関数】ゲーム全体のループを開始する
void play_opening_story(Character *hero);

// 【story.c に書く関数】裏ボス出現時の演出メッセージを表示する
void play_secret_boss_encounter(void);

// 【battle.c に書く関数】戦闘を開始する
// 戻り値: 0=勝利/1=敗北/2=魔王撃破（裏ボス撃破も含む）/3=セーブして終了
int start_battle(Character *hero);

// enemy.txt から指定ステージの敵データを読み込み、
// is_boss（魔王かどうか）もここでセットする
// stage=777 は低確率で出現する裏ボス専用ステージ番号
void load_enemy(int stage, Character *enemy);

#endif // GENERAL_RULES_H