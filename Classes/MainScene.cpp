//
//  MainScene.cpp
//  HelloCocos
//
//  Created by mitake on 2/11/15.
//
//

#include "MainScene.h"

USING_NS_CC;

// フルーツの画面上端からのマージン(40px)
const int FRUIT_TOP_MARGIN = 40;
// フルーツの出現率
const int FRUIT_SPAWN_PATE = 20;
// 制限時間
const float TIME_LIMIT_SECOND = 60;
// 黄金のフルーツを取った時の点数
const int GOLDEN_FRUIT_SCORE = 5;

MainScene::MainScene() :_player(NULL), _score(0), _scoreLabel(NULL), _second(TIME_LIMIT_SECOND), _secondLabel(NULL), _state(GameState::PLAYING){
}

MainScene::~MainScene(){
    CC_SAFE_RELEASE_NULL(_player);
    CC_SAFE_RELEASE_NULL(_scoreLabel);
    CC_SAFE_RELEASE_NULL(_secondLabel);
}

Scene* MainScene::createScene(){
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init(){
    if(!Layer::init()){
        return false;
    }
    
    // Director を取り出す
    auto director = Director::getInstance();
    // 画面サイズを取り出す
    auto size = director->getWinSize();
    // 背景のスプライトを生成する
    auto background = Sprite::create("background.png");
    // スプライトの表示位置を設定する
    background->setPosition(Vec2(size.width/2.0, size.height/2.0));
    // 親ノードにスプライトを追加する
    this->addChild(background);
    
    // Sprite を生成して _player に格納
    this->setPlayer(Sprite::create("player.png"));
    _player->setPosition(Vec2(size.width/2.0, size.height - 445));
    this->addChild(_player);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event){
        // タッチされた時の処理
        log("touch at (%f %f)", touch->getLocation().x, touch->getLocation().y);
        return true;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event){
        // タッチ中に動いた時の処理
        // 前回とのタッチ位置との差をベクトルで取得する
        Vec2 delta = touch->getDelta();
        
        // 現在の Player の座標を取得する
        Vec2 position = _player->getPosition();
        
        // 移動後の座標を算出する
        Vec2 newPosition = position + delta;
        auto winSize = Director::getInstance()->getWinSize();
        if(newPosition.x < 0){
            newPosition.x = 0;
        }else if(newPosition.x > winSize.width){
            newPosition.x = winSize.width;
        }
        newPosition.y = _player->getPositionY();
        _player->setPosition(newPosition);
    };
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // update を毎フレーム実行するように登録
    this->scheduleUpdate();
    
    // スコアラベルの追加
    auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score), "Marker Felt", 16);
    scoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    scoreLabel->enableOutline(Color4B::BLACK, 1.5);
    scoreLabel->setPosition(Vec2(size.width/2.0 * 1.5, size.height - 40));
    this->setScoreLabel(scoreLabel);
    this->addChild(_scoreLabel);
    
    // スコアヘッダーの追加
    auto scoreLabelHeader = Label::createWithSystemFont("SCORE", "Marker Felt", 16);
    scoreLabelHeader->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    scoreLabelHeader->enableOutline(Color4B::BLACK, 1.5);
    scoreLabelHeader->setPosition(Vec2(size.width/2.0*1.5, size.height - 20));
    this->addChild(scoreLabelHeader);
    
    // タイマーラベルの追加
    int second = static_cast<int>(_second);
    auto secondLabel = Label::createWithSystemFont(StringUtils::toString(second), "Marker Felt", 16);
    this->setSecondLabel(secondLabel);
    secondLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabel->enableOutline(Color4B::BLACK, 1.5);
    secondLabel->setPosition(Vec2(size.width/2.0, size.height - 40));
    this->addChild(secondLabel);
    
    // タイマーヘッダーの追加
    auto secondLabelHeader = Label::createWithSystemFont("TIME", "Marker Felt", 16);
    secondLabelHeader->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabelHeader->enableOutline(Color4B::BLACK, 1.5);
    secondLabelHeader->setPosition(Vec2(size.width/2.0, size.height - 20));
    this->addChild(secondLabelHeader);
    
    return true;
}

void MainScene::update(float dt){
    
    if(_state == GameState::PLAYING){
        // 毎フレーム実行される
        int random = rand() % FRUIT_SPAWN_PATE;
        if(random==0){
            this->addFruit();
        }
        
        for(auto& fruit : _fruits){
            Vec2 busketPosition = _player->getPosition() - Vec2(0, 10);
            Rect boundingBox = fruit->getBoundingBox();
            bool isHit = boundingBox.containsPoint(busketPosition);
            if(isHit){
                this->catchFruit(fruit);
            }
        }
    
        // 残り時間を減らす
        _second -= dt;
        // 残り秒数の表示を更新する
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::toString(second));
        
        if(_second < 0){
            // リザルト画面へ移行
            this->onResult();
        }
    }
}

Sprite* MainScene::addFruit(){
    // 画面サイズを取り出す
    auto winSize = Director::getInstance()->getWinSize();
    // フルーツの種類を選択する
    int fruitType = rand() % static_cast<int>(FruitType::COUNT);
    
    // フルーツを作成する
    std::string fileName = StringUtils::format("fruit%d.png", fruitType);
    auto fruit = Sprite::create(fileName);
    // フルーツの種類をタグとして登録
    fruit->setTag(fruitType);
    
    auto fruitSize = fruit->getContentSize();
    float fruitXPos = rand() % static_cast<int>(winSize.width);
    
    fruit->setPosition(Vec2(fruitXPos, winSize.height-FRUIT_TOP_MARGIN-fruitSize.height/2.0));
    this->addChild(fruit);
    _fruits.pushBack(fruit);
    
    // 地面の座標
    auto grand = Vec2(fruitXPos, 0);
    
    // 3 秒掛けて grand の位置まで落下させるアクション
    auto fall = MoveTo::create(3, grand);
    // removeFruit を呼び出すアクション
    auto remove = CallFuncN::create([this](Node* node){
        // Node を Sprite にダウンキャストする
        auto sprite = dynamic_cast<Sprite *>(node);
        this->removeFruit(sprite);
    });
    auto sequence = Sequence::create(fall, remove, NULL);
    fruit->runAction(sequence);
    
    return fruit;
}

bool MainScene::removeFruit(cocos2d::Sprite* fruit){
    // _fruits に fruit が含まれているかを確認する
    if(_fruits.contains(fruit)){
        // 親ノードから削除する
        fruit->removeFromParent();
        // _fruits 配列から削除する
        _fruits.eraseObject(fruit);
        return true;
    }
    return false;
}

void MainScene::catchFruit(cocos2d::Sprite* fruit){

    FruitType fruitType = static_cast<FruitType>(fruit->getTag());
    switch (fruitType) {
        case MainScene::FruitType::GOLDEN:
            // 黄金のフルーツを取った時の処理
            _score += GOLDEN_FRUIT_SCORE;
            break;
        case MainScene::FruitType::BOMB:
            // 爆弾を取った時の処理
            break;
        default:
            // その他のフルーツのとき
            _score += 1;
            break;
    }
    
    // フルーツを削除する
    this->removeFruit(fruit);

    // スコア表示の更新
    _scoreLabel->setString(StringUtils::toString(_score));
}

void MainScene::onResult(){
    _state = GameState::RESULT;
    auto winSize = Director::getInstance()->getWinSize();
    
    // 「もう一度遊ぶ」ボタン
    auto replayButton = MenuItemImage::create("replay_button.png", "replay_button_pressed.png", [](Ref* ref){
        // 新しく MainScene を作成して置き換えてやる
        auto scene = MainScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });
    
    // 「タイトルへ戻る」ボタン
    auto titleButton = MenuItemImage::create("title_button.png", "tittle_button_pressed.png", [](Ref* ref){
        //
    });
    
    // 2 個のボタンからメニューを作成する
    auto menu = Menu::create(replayButton, titleButton, NULL);
    // ボタンを縦に並べる
    menu->alignItemsVerticallyWithPadding(15);
    menu->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
    this->addChild(menu);
}