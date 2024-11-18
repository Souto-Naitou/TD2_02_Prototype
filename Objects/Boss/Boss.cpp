#include "Boss.h"

#include <ModelManager.h>
#include <fstream>

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

    // 通常攻撃発生ファイル読み込み
    LoadNormalAttackPopData();
}

void Boss::Update()
{
    //デスフラグの立った弾を削除
    bullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });

    Vector3 point1 = { -2.0f, 0.0f, 5.0f };
    Vector3 point2 = { 2.0f, 0.0f, 5.0f };

    if (easing_->GetIsEnd())
    {
        easing_->Reset();
    }

    position_.Lerp(point1, point2, easing_->Update());

    object_->SetPosition(position_);
    object_->Update();

    //NormalAttack();
    UpdateNormalAttackPopCommands();

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }
}

void Boss::Draw()
{
    object_->Draw();

    // 弾描画
    for (auto& bullet : bullets_) {
        bullet->Draw();
    }
}

void Boss::Finalize()
{
    for (auto& bullet : bullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
        //delete bullet;
    }

    bullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });
}

void Boss::NormalAttack()
{
    // 弾を生成し、初期化
    BossNormalBullet* newBullet = new BossNormalBullet();

    newBullet->SetPosition(position_);
    newBullet->SetPlayerPosition(playerPosition_);
    newBullet->Initialize();
    newBullet->SetVelocity(bltVelocity_);

    // 弾を登録する
    bullets_.push_back(newBullet);
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
    if (isWaiting) {
        waitingTimer--;

        if (waitingTimer <= 0) {
            //待機完了
            isWaiting = false;
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
            isWaiting = true;
            waitingTimer = waitTime;

            // 通常攻撃発生
            NormalAttack();

            //コマンドループを抜ける
            break;
        }
    }
}
