#include "SceneFactory.h"

#include "TitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	}
	if (sceneName == "GAMEPLAY") {
		newScene = new GamePlayScene();
	}
	if (sceneName == "GAMECLAE") {
		newScene = new GameClearScene();
	}
	if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	}

	return newScene;
}
