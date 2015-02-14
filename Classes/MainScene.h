//
//  MainScene.h
//  HelloCocos
//
//  Created by mitake on 2/11/15.
//
//

#ifndef __HelloCocos__MainScene__
#define __HelloCocos__MainScene__

#include <stdio.h>
#include <cocos2d.h>


class MainScene :public cocos2d::Layer
{
protected:
    MainScene();
    virtual ~MainScene();
    bool init() override;
public:
    static cocos2d::Scene* createScene();
    void update(float dt);
    CREATE_FUNC(MainScene);
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);
    CC_SYNTHESIZE(float, _second, Second);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);
    
private:
    // フルーツの種類を定義する
    enum class FruitType{
        APPLE,  // りんご
        GRAPE,  // ぶどう
        ORANGE, // みかん
        BANANA, // ばなな
        CHERRY, // さくらんぼ
        COUNT   // 最大数
    };
    
    /** 画面にフルーツを新たに配置して、それを返却する
     * @return 新たに作成されたフルール
     */
    cocos2d::Sprite* addFruit();
    
    
    /** 画面からフルーツを取り除く
     * @param fruit 削除するフルーツ
     * @param return 正しく削除されたか
     */
    bool removeFruit(cocos2d::Sprite* fruit);
    
    /** フルーツを取得する
     * @param Sprite* 取得するフルーツ
     */
    void catchFruit(cocos2d::Sprite* fruit);
     
};

#endif /* defined(__HelloCocos__MainScene__) */
