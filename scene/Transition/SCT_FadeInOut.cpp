#include "SCT_FadeInOut.h"

void SCT_FadeInOut::Initialize(const std::string& _sceneName)
{
    sceneName_ = _sceneName;
    sprite_.Initialize("Resources/images/white1x1.png", 0, { 0,0,0,1 });
    timer_.Start();
}

void SCT_FadeInOut::Update()
{

}

void SCT_FadeInOut::Draw()
{

}

void SCT_FadeInOut::Finalize()
{
}
