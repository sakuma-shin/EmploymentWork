#pragma once

enum Scene { TITLE, STAGE,RESULT };

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
