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

MainScene::MainScene() :_player(NULL){
}

MainScene::~MainScene(){
    CC_SAFE_RELEASE_NULL(_player);
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
    return true;
}

void MainScene::update(float dt){
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
    // フルーツを削除する
    this->removeFruit(fruit);
}