//
//  Home.cpp
//  CoolRun
//
//  Created by ManYou on 14/11/6.
//
//

#include "Home.h"
#include "ActionHelp.h"

Home::Home()
: Land()
, m_door(nullptr)
{
}
Home::~Home()
{
}
bool Home::init()
{
    if (!Land::init())
    {
        return false;
    }
    
    auto homeSp = Sprite::create("home.png");
    auto csize = homeSp->getContentSize();
    this->setContentSize(csize);
    
    homeSp->setAnchorPoint(Vec2::ZERO);
    this->addChild(homeSp);
    
    this->setCollideRect(Rect(0, 0, 1220, 230));
    this->setCollideEffect(true);
    this->setCollideType(kCollideTypeDirection);
    this->setGravityEffect(false);
    
    m_door = Sprite::createWithSpriteFrameName("door_01.png");
    m_door->setAnchorPoint(Vec2::ZERO);
    m_door->setPosition(Vec2(246, 218));
    this->addChild(m_door);
    
    
    this->debugShow();
    
    return true;
}

void Home::openDoor()
{
    m_door->stopAllActions();
    auto action = ActionHelp::createFrameAction("door_%02d.png", 1, 3, 0.2, false);
    m_door->runAction(action);
}
void Home::closeDoor()
{
    m_door->stopAllActions();
    auto action = ActionHelp::createFrameAction("door_%02d.png", 3, 1, 0.2, false);
    m_door->runAction(action);
}