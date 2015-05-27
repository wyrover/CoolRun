//
//  MYUserDefaultManager.h
//  CoolRun
//
//  Created by ManYou on 15/5/13.
//
//

#ifndef __CoolRun__MYUserDefaultManager__
#define __CoolRun__MYUserDefaultManager__

#include "cocos2d.h"
#include "MYMultiLanguageManager.h"

//UserDefault
#define IS_PLAYER_GUIDED "is_player_guided"
#define MULTI_LANGUAGE_FLAG "current_language"
#define IS_EFFECT_OFF "is_effect_off"
#define IS_MUSIC_OFF "is_music_off"

USING_NS_CC;
using namespace std;

class MYUserDefaultManager : public Ref
{
private:
    MYUserDefaultManager();
    ~MYUserDefaultManager();
public:
    virtual bool init();
    
    static MYUserDefaultManager* getInstance();
    static void purgeInstance();
    
    bool isPlayedGuide();
    void setPlayedGuide(bool key);
    
    MYMultiLanguageManager::MultiLanguageType getCurrentLanguage();
    void setCurrentLanguage(MYMultiLanguageManager::MultiLanguageType _type);
    
    bool isEffectOff();
    void setEffectOff(bool flag);
    bool isMusicOff();
    void setMusicOff(bool flag);
    
private:
    static MYUserDefaultManager* m_instance;
};

#endif /* defined(__CoolRun__MYUserDefaultManager__) */
