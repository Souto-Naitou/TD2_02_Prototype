#include "Boss.h"

#include <ModelManager.h>
#include "ImGuiDebugManager/DebugManager.h"
#include <fstream>
#include "Feature/RandomGenerator/RandomGenerator.h"

#include "State/BossStateFirst.h"
#include "State/BossStateSecond.h"
#include "State/BossStateThird.h"
#include "State/BossStateFourth.h"

#ifdef _DEBUG

#include "imgui.h"
#include <Helper/ImGuiTemplates/ImGuiTemplates.h>

#endif // _DEBUG

void Boss::Initialize()
{
    /// インスタンスの初期化
    CSVLoader* csvLoader = CSVLoader::GetInstance();
    DebugManager* pDebugManager = DebugManager::GetInstance();
    pTimer_ = std::make_unique<Timer>();

    pDebugManager->SetComponent("Boss", std::bind(&Boss::DebugWindow, this));


    csvData_ = csvLoader->LoadFile("Boss.csv");

    /// Transformの初期化
    scale_ = { 0.5f, 0.5f, 0.5f };
    position_ = { 0.0f, 0.0f, 10.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    ModelManager::GetInstance()->LoadModel("Boss/boss.obj");

    object_->Initialize("boss.obj");
    object_->SetSize(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);

    easing_ = std::make_unique<Easing>("TEST");
    easing_->Initialize();

    this->RegisterDebugWindow();

    // HPマックスにする
    hp_ = kMaxHitPoint;

    // ステート
    ChangeState(std::make_unique<BossStateFirst>(this));


    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Boss";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Boss::OnCollision, this));
    collisionManager_->RegisterCollider(&collider_);

    pTimer_->Start();
}

void Boss::Update()
{
    // ステート(フラグ取得の関係で上に移動)
    pState_->Attack();

    // デスフラグの立った弾を削除
    DeleteBullet();

    UpdateRandomMovement();

    object_->SetPosition(position_);
    object_->Update();

    aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);

    OutputCSV();

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

    // 歌描画
    for (auto& bullet : pSongBullets_) {
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

    // 歌終了
    for (auto& bullet : pSongBullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
    }

    DeleteBullet();
    collisionManager_->DeleteCollider(&collider_);

    DebugManager::GetInstance()->DeleteComponent("Boss");
}

void Boss::NormalAttack()
{
    // 通常弾を生成し、初期化
    BossNormalBullet* newBullet = new BossNormalBullet();

    newBullet->SetPosition(position_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "BossNormal", "BossMoon"));
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

    this->UnregisterDebugWindow();

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

    // すべての行を読み終えた場合にリセット
    if (normalAttackPopCommands.eof()) {
        ResetNormalAttackPopCommands(); // リセット処理
    }
}

void Boss::ResetNormalAttackPopCommands()
{
    // ストリームの状態をリセットして、最初の位置に戻す
    normalAttackPopCommands.clear(); // ストリーム状態フラグをクリア
    normalAttackPopCommands.seekg(0, std::ios::beg); // ストリームの読み取り位置を先頭に設定
}

void Boss::PillowAttack()
{
    // 枕弾を生成し、初期化
    BossPillow* newBullet = new BossPillow();

    newBullet->SetPosition(position_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "BossPillow", "BossMoon"));
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

    // すべての行を読み終えた場合にリセット
    if (pillowAttackPopCommands.eof()) {
        ResetPillowAttackPopCommands(); // リセット処理
    }
}

void Boss::ResetPillowAttackPopCommands()
{
    // ストリームの状態をリセットして、最初の位置に戻す
    pillowAttackPopCommands.clear(); // ストリーム状態フラグをクリア
    pillowAttackPopCommands.seekg(0, std::ios::beg); // ストリームの読み取り位置を先頭に設定
}

void Boss::MoonAttack()
{
    // 枕弾を生成し、初期化
    BossMoon* newBullet = new BossMoon();

    newBullet->SetPosition(position_);
    newBullet->SetRotation(moonRotate_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "BossMoon"));
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

    // すべての行を読み終えた場合にリセット
    if (moonAttackPopCommands.eof()) {
        ResetMoonPopCommands(); // リセット処理
    }
}

void Boss::ResetMoonPopCommands()
{
    // ストリームの状態をリセットして、最初の位置に戻す
    moonAttackPopCommands.clear(); // ストリーム状態フラグをクリア
    moonAttackPopCommands.seekg(0, std::ios::beg); // ストリームの読み取り位置を先頭に設定
}

void Boss::SongAttack()
{
    // 歌を生成し、初期化
    BossSong* newBullet = new BossSong();

    newBullet->SetPosition(position_);
    newBullet->Initialize();

    // 歌を登録する
    pSongBullets_.push_back(newBullet);
}

void Boss::LoadSongPopData()
{
    //ファイルを開く
    std::ifstream file;
    file.open("Resources/CSV/BossSongPop.csv");
    assert(file.is_open());

    //ファイルの内容を文字列ストリームにコピー
    songAttackPopCommands << file.rdbuf();

    //ファイルを閉じる
    file.close();
}

void Boss::UpdateSongPopCommands()
{
    //待機処理
    if (isSongWaiting_) {
        songWaitingTimer_--;

        if (songWaitingTimer_ <= 0) {
            //待機完了
            isSongWaiting_ = false;
        }
        return;
    }


    //1行分の文字列を入れる変数
    std::string line;

    //コマンドループ
    while (getline(songAttackPopCommands, line)) {
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
            isSongWaiting_ = true;
            songWaitingTimer_ = waitTime;

            // 歌攻撃発生
            SongAttack();

            //コマンドループを抜ける
            break;
        }
    }

    // すべての行を読み終えた場合にリセット
    if (songAttackPopCommands.eof()) {
        ResetSongPopCommands(); // リセット処理
    }
}

void Boss::ResetSongPopCommands()
{
    // ストリームの状態をリセットして、最初の位置に戻す
    songAttackPopCommands.clear(); // ストリーム状態フラグをクリア
    songAttackPopCommands.seekg(0, std::ios::beg); // ストリームの読み取り位置を先頭に設定
}

void Boss::DeleteBullet()
{
    // デスフラグの立った通常弾を削除
    pNormalBullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });

    // デスフラグの立った枕弾を削除
    pPillowBullets_.remove_if([](BossPillow* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });

    // デスフラグの立った月を削除
    pMoonBullets_.remove_if([](BossMoon* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });

    // デスフラグの立った歌を削除
    pSongBullets_.remove_if([](BossSong* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });
}

void Boss::ChangeState(std::unique_ptr<BaseBossState> _pState)
{
    pState_ = std::move(_pState);
}

void Boss::OnCollision()
{
    if (hp_ > 0)
    {
        hp_ -= 1;
    }
    else
    {
        isBossDeadMoment_ = true;
    }
}

void Boss::DebugWindow()
{
    auto pFunc = [&]() {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("Scale", scale_);
        ImGuiTemplate::VariableTableRow("Rotation", rotation_);
        ImGuiTemplate::VariableTableRow("HP", hp_);
        ImGuiTemplate::VariableTableRow("DestPosition", destPosition_);
        ImGuiTemplate::VariableTableRow("moveTimer", pTimer_->GetNow());
    };

    ImGuiTemplate::VariableTable("Boss",pFunc);

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

}

void Boss::OutputCSV()
{
    CSVLoader* csvLoader = CSVLoader::GetInstance();
    CSVLine* csvLine = nullptr;


    /// スケールの書き込み
    csvLine = csvLoader->GetLine("Boss.csv", "Scale");
    if (!csvLine)
    {
        csvLine = csvLoader->GetNewLine("Boss.csv");
        csvLine->resize(4);
    }
    else if (csvLine->size() != 4)
    {
        csvLine->clear();
        csvLine->resize(4);
    }

    for (auto& word : *csvLine) {
        word.str("");
        word.clear();
    }

    (*csvLine)[0] << "Scale";
    (*csvLine)[1] << scale_.x;
    (*csvLine)[2] << scale_.y;
    (*csvLine)[3] << scale_.z;


    /// 回転の書き込み
    csvLine = csvLoader->GetLine("Boss.csv", "Rotation");
    if (!csvLine)
    {
        csvLine = csvLoader->GetNewLine("Boss.csv");
        csvLine->resize(4);
    }
    else if (csvLine->size() != 4)
    {
        csvLine->clear();
        csvLine->resize(4);
    }

    for (auto& word : *csvLine) {
        word.str("");
        word.clear();
    }

    (*csvLine)[0] << "Rotation";
    (*csvLine)[1] << rotation_.x;
    (*csvLine)[2] << rotation_.y;
    (*csvLine)[3] << rotation_.z;


    /// 位置の書き込み
    csvLine = csvLoader->GetLine("Boss.csv", "Position");
    if (!csvLine)
    {
        csvLine = csvLoader->GetNewLine("Boss.csv");
        csvLine->resize(4);
    }
    else if (csvLine->size() != 4)
    {
        csvLine->clear();
        csvLine->resize(4);
    }

    for (auto& word : *csvLine) {
        word.str("");
        word.clear();
    }

    (*csvLine)[0] << "Position";
    (*csvLine)[1] << position_.x;
    (*csvLine)[2] << position_.y;
    (*csvLine)[3] << position_.z;


    csvLoader->SaveFile();
}

void Boss::UpdateRandomMovement()
{
    if (pTimer_->GetNow() > 5.0)
    {
        Vector3 pos0 = Vector3(-10.0f, 0, -10.0f);
        Vector3 pos1 = Vector3(10.0f, 0, 10.0f);

        // 乱数生成
        destPosition_ = RandomGenerator::GetRandom(pos0, pos1);
        pTimer_->Reset();
        pTimer_->Start();
    }

    position_.Lerp(position_, destPosition_, 0.01f);
}
