#include "Boss.h"

#include <ModelManager.h>
#include <fstream>

#include "State/BossStateFirst.h"
#include "State/BossStateSecond.h"
#include "State/BossStateThird.h"
#include "State/BossStateFourth.h"

#ifdef _DEBUG

#include "imgui.h"

#endif // _DEBUG


void Boss::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("cube.obj");

    /// Transformの初期化
    scale_ = { 0.5f, 0.5f, 0.5f };
    position_ = { 0.0f, 0.0f, 10.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("cube.obj");
    object_->SetSize(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);

    easing_ = std::make_unique<Easing>("TEST");
    easing_->Initialize();

    // HPマックスにする
    hitPoint_ = kMaxHitPoint;

    // ステート
    ChangeState(std::make_unique<BossStateFirst>(this));
}

void Boss::Update()
{
    // デスフラグの立った弾を削除
    DeleteBullet();


    Vector3 point1 = { -2.0f, 0.0f, 5.0f };
    Vector3 point2 = { 2.0f, 0.0f, 5.0f };

    if (easing_->GetIsEnd())
    {
        easing_->Reset();
    }

    position_.Lerp(point1, point2, easing_->Update());

    object_->SetPosition(position_);
    object_->Update();

   

    // ステート
    pState_->Attack();

    // 通常弾更新
    for (auto& bullet : pNormalBullets_) {
        bullet->Update();
    }

    // 枕弾更新
    for (auto& bullet : pPillowBullets_) {
        bullet->Update();
    }

    // 枕弾更新
    for (auto& bullet : pMoonBullets_) {
        bullet->Update();
    }

#ifdef _DEBUG

    ImGui::Begin("Easing Parameters", nullptr);

    ImGui::Text("Select Boss State");
    if (ImGui::RadioButton("BossStateFirst", selectState_ == 0)) {
        selectState_ = 0;
        ChangeState(std::make_unique<BossStateFirst>(this));
    }
    if (ImGui::RadioButton("BossStateSecond", selectState_ == 1)) {
        selectState_ = 1;
        ChangeState(std::make_unique<BossStateSecond>(this));
    }
    if (ImGui::RadioButton("BossStateThird", selectState_ == 2)) {
        selectState_ = 2;
        ChangeState(std::make_unique<BossStateThird>(this));
    }
    if (ImGui::RadioButton("BossStateFourth", selectState_ == 3)) {
        selectState_ = 3;
        ChangeState(std::make_unique<BossStateFourth>(this));
    }

    ImGui::End();

#endif // _DEBUG


}

void Boss::Draw()
{
    object_->Draw();

    // 通常弾描画
    for (auto& bullet : pNormalBullets_) {
        bullet->Draw();
    }

    // 枕描画
    for (auto& bullet : pPillowBullets_) {
        bullet->Draw();
    }

    // 月描画
    for (auto& bullet : pMoonBullets_) {
        bullet->Draw();
    }
}

void Boss::Finalize()
{
    // 通常弾終了
    for (auto& bullet : pNormalBullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
    }

    // 枕終了
    for (auto& bullet : pPillowBullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
    }

    // 月終了
    for (auto& bullet : pMoonBullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
    }

    DeleteBullet();
}

void Boss::NormalAttack()
{
    // 通常弾を生成し、初期化
    BossNormalBullet* newBullet = new BossNormalBullet();

    newBullet->SetPosition(position_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    newBullet->SetVelocity(bltVelocity_);

    // 通常弾を登録する
    pNormalBullets_.push_back(newBullet);
}

void Boss::LoadNormalAttackPopData()
{
    //ファイルを開く
    std::ifstream file;
    file.open("Resources/CSV/BossNormalBulletPop.csv");
    assert(file.is_open());

    //ファイルの内容を文字列ストリームにコピー
    normalAttackPopCommands << file.rdbuf();

    //ファイルを閉じる
    file.close();
}

void Boss::UpdateNormalAttackPopCommands()
{

    //待機処理
    if (isNormalWaiting_) {
        normalWaitingTimer_--;

        if (normalWaitingTimer_ <= 0) {
            //待機完了
            isNormalWaiting_ = false;
        }
        return;
    }


    //1行分の文字列を入れる変数
    std::string line;

    //コマンドループ
    while (getline(normalAttackPopCommands, line)) {
        //1行分の文字列を入れる変数
        std::istringstream line_stream(line);

        std::string word;
        // ,区切りで行の先頭列を取得
        getline(line_stream, word, ',');

        // "//"から始まる行はコメント
        if (word.find("//") == 0) {
            //コメント行を飛ばす
            continue;
        }

      
        // WAITコマンド
        if (word.find("WAIT") == 0) {

            getline(line_stream, word, ',');

            // 待ち時間
            int32_t waitTime = atoi(word.c_str());

            //待機時間
            isNormalWaiting_ = true;
            normalWaitingTimer_ = waitTime;

            // 通常攻撃発生
            NormalAttack();

            //コマンドループを抜ける
            break;
        }
    }
}

void Boss::PillowAttack()
{
    // 枕弾を生成し、初期化
    BossPillow* newBullet = new BossPillow();

    newBullet->SetPosition(position_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    newBullet->SetVelocity(bltVelocity_);

    // 枕弾を登録する
    pPillowBullets_.push_back(newBullet);
}

void Boss::LoadPillowPopData()
{
    //ファイルを開く
    std::ifstream file;
    file.open("Resources/CSV/BossPillowPop.csv");
    assert(file.is_open());

    //ファイルの内容を文字列ストリームにコピー
    pillowAttackPopCommands << file.rdbuf();

    //ファイルを閉じる
    file.close();
}

void Boss::UpdatePillowPopCommands()
{
    //待機処理
    if (isPillowWaiting_) {
        pillowWaitingTimer_--;

        if (pillowWaitingTimer_ <= 0) {
            //待機完了
            isPillowWaiting_ = false;
        }
        return;
    }


    //1行分の文字列を入れる変数
    std::string line;

    //コマンドループ
    while (getline(pillowAttackPopCommands, line)) {
        //1行分の文字列を入れる変数
        std::istringstream line_stream(line);

        std::string word;
        // ,区切りで行の先頭列を取得
        getline(line_stream, word, ',');

        // "//"から始まる行はコメント
        if (word.find("//") == 0) {
            //コメント行を飛ばす
            continue;
        }


        // WAITコマンド
        if (word.find("WAIT") == 0) {

            getline(line_stream, word, ',');

            // 待ち時間
            int32_t waitTime = atoi(word.c_str());

            //待機時間
            isPillowWaiting_ = true;
            pillowWaitingTimer_ = waitTime;

            // 枕攻撃発生
            PillowAttack();

            //コマンドループを抜ける
            break;
        }
    }
}

void Boss::MoonAttack()
{
    // 枕弾を生成し、初期化
    BossMoon* newBullet = new BossMoon();

    newBullet->SetPosition(position_);
    newBullet->SetRotation(moonRotate_);
    newBullet->Initialize();
    newBullet->SetVelocity(bltVelocity_);

    // 枕弾を登録する
    pMoonBullets_.push_back(newBullet);
}

void Boss::LoadMoonPopData()
{
    //ファイルを開く
    std::ifstream file;
    file.open("Resources/CSV/BossMoonPop.csv");
    assert(file.is_open());

    //ファイルの内容を文字列ストリームにコピー
    moonAttackPopCommands << file.rdbuf();

    //ファイルを閉じる
    file.close();
}

void Boss::UpdateMoonPopCommands()
{
    //待機処理
    if (isMoonWaiting_) {
        moonWaitingTimer_--;

        if (moonWaitingTimer_ <= 0) {
            //待機完了
            isMoonWaiting_ = false;
        }
        return;
    }


    //1行分の文字列を入れる変数
    std::string line;

    //コマンドループ
    while (getline(moonAttackPopCommands, line)) {
        //1行分の文字列を入れる変数
        std::istringstream line_stream(line);

        std::string word;
        // ,区切りで行の先頭列を取得
        getline(line_stream, word, ',');

        // "//"から始まる行はコメント
        if (word.find("//") == 0) {
            //コメント行を飛ばす
            continue;
        }

        // ROTATEコマンド
        if (word.find("ROTATE") == 0) {
            // X座標
            getline(line_stream, word, ',');
            float x = (float)std::atof(word.c_str());

            // Y座標
            getline(line_stream, word, ',');
            float y = (float)std::atof(word.c_str());

            // Z座標
            getline(line_stream, word, ',');
            float z = (float)std::atof(word.c_str());

            // 度数法からラジアンに変換
            x *= ((float)std::numbers::pi / 180);
            y *= ((float)std::numbers::pi / 180);
            z *= ((float)std::numbers::pi / 180);

            // 月を回転させる
            moonRotate_ = {x, y, z};

        }
        // WAITコマンド
        else if (word.find("WAIT") == 0) {

            getline(line_stream, word, ',');

            // 待ち時間
            int32_t waitTime = atoi(word.c_str());

            //待機時間
            isMoonWaiting_ = true;
            moonWaitingTimer_ = waitTime;

            // 月攻撃発生
            MoonAttack();

            //コマンドループを抜ける
            break;
        }
    }
}

void Boss::DeleteBullet()
{
    // デスフラグの立った通常弾を削除
    pNormalBullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });

    // デスフラグの立った枕弾を削除
    pPillowBullets_.remove_if([](BossPillow* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });

    // デスフラグの立った月を削除
    pMoonBullets_.remove_if([](BossMoon* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });
}

void Boss::ChangeState(std::unique_ptr<BaseBossState> _pState)
{
    pState_ = std::move(_pState);
}
