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
    CREATE_FUNC(MainScene);
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player);
};

#endif /* defined(__HelloCocos__MainScene__) */
