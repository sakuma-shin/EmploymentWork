#pragma once

enum Scene { TITLE, STAGE,RESULT };

/// <summary>
/// シーンの共通処理をまとめた親クラス
/// </summary>
class IScene {
protected:
	static int sceneNo;

public:
	virtual void Initialize()=0;

	virtual void Update()=0;

	virtual void Draw()=0;

	virtual ~IScene();

	//シーン番号のゲッター
	int GetSceneNo();

};
