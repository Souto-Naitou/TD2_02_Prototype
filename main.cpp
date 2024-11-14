#include "MyGame.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "00_01.lib")
#pragma comment(lib, "easing.lib")

///////////////////////////////////////////////////////////

// Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Framework* game = new MyGame();

	game->Run();

	delete game;

	return 0;

}