#pragma once

#include <functional>
#include <list>
#include <string>
#include <tuple>

#include <Timer/Timer.h>

#include <ImGuiTemplates/ImGuiTemplates.h>

class DebugManager
{
public:
    static DebugManager* GetInstance() { static DebugManager instance; return &instance; }

    DebugManager(const DebugManager&)               = delete;
    DebugManager& operator=(const DebugManager&)    = delete;
    DebugManager(DebugManager&&)                    = delete;
    DebugManager& operator=(const DebugManager&&)   = delete;

    /// <summary>
    /// デバッグ用コンポーネントの登録
    /// </summary>
    /// <param name="_strID">タブに表示される名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string _strID, const std::function<void(void)>& _component)
    {
        componentList_.push_back(std::make_tuple(std::string("null-name"), _strID, _component, false));
    }

    /// <summary>
    /// デバッグ用コンポーネントの登録 (リスト用)
    /// </summary>
    /// <param name="_parentID">オブジェクトの種類</param>
    /// <param name="_childID">オブジェクトの名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string _parentID, std::string _childID, const std::function<void(void)>& _component)
    {
        for (auto& comp : componentList_)
        {
            if (std::get<1>(comp) == _childID)
            {
                return;
            }
        }

        componentList_.emplace(
            GetInsertIterator(_parentID),
            _parentID,
            _childID,
            _component,
            false
        );
    }

    void DeleteComponent(const char* _strID);
    void DeleteComponent(const char* _parentID, const char* _childID);

    void DrawUI();
    void ChangeFont();
    void SetDisplay(bool _isEnable) { onDisplay_ = _isEnable; }

private:
    DebugManager();
    ~DebugManager();

    std::list<std::tuple<std::string, std::string, const std::function<void(void)>, bool>> componentList_;
    Timer           timer_              = {};
    double          elapsedFrameCount_  = 0.0;
    double          fps_                = 0.0;
    unsigned int    frameCount_         = 0u;
    bool            onDisplay_          = true;

private:
    void DebugWindowOverall();
    void MeasureFPS();
    void Window_ObjectList();
    std::list<std::tuple<std::string,std::string,const std::function<void(void)>,bool>>::iterator
        GetInsertIterator(std::string _parentName);
};