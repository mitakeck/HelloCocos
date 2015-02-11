//
//  MainScene.cpp
//  HelloCocos
//
//  Created by mitake on 2/11/15.
//
//

#include "MainScene.h"

USING_NS_CC;

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
    
    return true;
}

