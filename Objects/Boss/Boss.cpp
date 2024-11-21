#include "Boss.h"

#include <ModelManager.h>
#include "Collision/CollisionManager/CollisionManager.h"


void Boss::Initialize()
{
    CollisionManager* collisionManager = CollisionManager::GetInstance();
    CSVLoader* csvLoader = CSVLoader::GetInstance();

    objectName_ = "Boss";

    csvData_ = csvLoader->LoadFile("Boss.csv");

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

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetAttribute(collisionManager->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetShape(Shape::AABB);
    collisionManager->RegisterCollider(&collider_);

    this->RegisterDebugWindow();
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

    NormalAttack();

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }

    aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);

    OutputCSV();
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
    if (bltCoolTime_ <= 0)
    {
        // 弾を生成し、初期化
        BossNormalBullet* newBullet = new BossNormalBullet();

        newBullet->SetPosition(position_);
        newBullet->SetPlayerPosition(playerPosition_);
        newBullet->Initialize();
        newBullet->SetVelocity(bltVelocity_);

        // 弾を登録する
        bullets_.push_back(newBullet);

        bltCoolTime_ = kBltCoolTime;
    }
    CollisionManager::GetInstance()->DeleteCollider(&collider_);

    this->UnregisterDebugWindow();
}

    bltCoolTime_--;
void Boss::RunSetMask()
{
    /// マスクの設定 (自分(指定されたid)は当たらない)
    collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID()));
}

void Boss::DebugWindow()
{
    auto pFunc = [&]() {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("Scale", scale_);
        ImGuiTemplate::VariableTableRow("Rotation", rotation_);
        ImGuiTemplate::VariableTableRow("HP", hp_);
    };

    ImGuiTemplate::VariableTable("Boss",pFunc);
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
