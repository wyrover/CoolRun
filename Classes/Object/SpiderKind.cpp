//
//  SpiderKind.cpp
//  CoolRun
//
//  Created by ManYou on 14/11/26.
//
//

#include "SpiderKind.h"
#include "Runner.h"
#include "Bullet.h"


SpiderKind::SpiderKind()
: Spider()
{
}
SpiderKind::~SpiderKind()
{
}
bool SpiderKind::init()
{
    if (!Spider::init())
    {
        return false;
    }
    
    auto csize = this->getContentSize();
    
    this->setCollideRect(Rect(40, 20, csize.width * 0.8f, csize.height * 0.8));
    
    this->debugShow();
    
    return true;
}

void SpiderKind::setState(SpiderState state)
{
    Spider::setState(state);
    switch (m_state) {
        case kSpiderState_Attack:
            m_armature->getAnimation()->play("attack02");
            break;
        case kSpiderState_Stand:
            m_armature->getAnimation()->play("stand02");
            break;
        case kSpiderState_Hurted:
            m_armature->getAnimation()->play("hurt02");
            break;
        case kSpiderState_Walk:
            m_armature->getAnimation()->play("walk02");
            break;
        default:
            break;
    }
    m_armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(SpiderKind::movementEvent, this));
    m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_4(SpiderKind::frameEvent, this));
}

void SpiderKind::dead()
{
    this->setState(kSpiderState_Hurted);
    this->setCollideEffect(false);
}

#pragma mark - bone callback
void SpiderKind::movementEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (MovementEventType::COMPLETE == movementType)
    {
        if("hurt02" == movementID)
        {
            this->setDestoryed(true);
        }
        
    }
}
void SpiderKind::frameEvent(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
}

#pragma mark - collide

void SpiderKind::trackCollideWithRunner(Runner* _runner)
{
    
    Spider::trackCollideWithRunner(_runner);
    if (!this->isCollideEffect() || this->isDestoryed())
    {
        return;
    }
    //处理与玩家的碰撞
    auto rect1 = PhysicHelp::countPhysicNodeRect(this);
    if (_runner->isAtk())
    {
        auto rect2 = PhysicHelp::countPhysicNodeRect(_runner, _runner->getAtkRect());
        
        bool isAtked = CollideTrackHelp::trackCollide(rect1, rect2);
        if (isAtked)
        {
            this->dead();
            return;
        }
        
    }
    
    auto rect2 = PhysicHelp::countPhysicNodeRect(_runner);
    
    CollideDirection dir = CollideTrackHelp::trackCollideDirection(rect1, rect2);
    
    if (kCollideDirectionUp == dir)
    {
        _runner->rebound();
        
        this->dead();
    }
    else if (kCollideDirectionMiss != dir)
    {
        m_gameController->dead(_runner);
        
//        this->setCollideEffect(false);
    }
}

void SpiderKind::trackCollideWithBullet(Bullet* bullet)
{
    auto rect1 = PhysicHelp::countPhysicNodeRect(bullet);
    auto rect2 = PhysicHelp::countPhysicNodeRect(this);
    
    bool isCollided = CollideTrackHelp::trackCollide(rect1, rect2);
    if (isCollided)
    {
        this->dead();
        bullet->setDestoryed(true);
    }
}


