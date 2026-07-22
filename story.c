git add General_Rules.h battle.c main.c story.c skills.txt
git status  # 意図通りのファイルだけがステージされているか確認
git commit -m "feat: 技システム(Skill構造体)とMP管理を追加、既存バグを修正

- Character構造体にmax_mp/skill_id/skill_count/is_bossを追加
- Skill構造体を新規追加、skills.txtから技データを読み込む処理を実装
- セーブ/ロードのフィールド数不一致によるexp消失バグを修正
- 名前入力・ロード時のバッファオーバーラン対策(%49s)
- 敵の技が0件の場合の0除算を防止
- srandの呼び出しをmain.c起動時の1回のみに統一"

git push -u origin feature/battle-system