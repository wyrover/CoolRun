//
//  EditorScene.cpp
//  CoolRun
//
//  Created by ManYou on 15/1/28.
//
//

#include "EditorScene.h"
#include "EditorUIBase.h"
#include "EditorZOrder.h"
#include "MYTextInput.h"
#include "EditorPage.h"
#include "EditorScrollMenu.h"
#include "EditorMainMenu.h"
#include "EditorPhysicNodeContainer.h"
#include "JsonHelp.h"
#include "SceneHelp.h"
#include "MissionPage.h"
#include <sys/stat.h>
//#include <fstream>
//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>

#include "Mission.h"
#include "CoolRunScene.h"
#include "MYScene.h"
#include "MYMultiLanguageManager.h"


#define kEditorTempFile  "Temp"

EditorScene::EditorScene()
: Layer()
, m_scrollView(nullptr)
, m_content(nullptr)
, m_firstMenuContainer(nullptr)
, m_state(MenuState::NONE)
, m_secondMenuContainer(nullptr)
, m_thirdMenuContainer(nullptr)
, m_curPage(nullptr)
, m_fileName(nullptr)
, m_fileData(nullptr)
, m_mission(nullptr)
, m_editorPages(nullptr)
{
}
EditorScene::~EditorScene()
{
    if (m_fileName)
    {
        delete m_fileName;
        m_fileName = nullptr;
    }
    if (m_fileData)
    {
        delete m_fileData;
        m_fileData = nullptr;
    }
    
    CC_SAFE_RELEASE_NULL(m_mission);
    CC_SAFE_RELEASE_NULL(m_editorPages);
}

bool EditorScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_editorPages = __Array::create();
    CC_SAFE_RETAIN(m_editorPages);
    
    auto layer = LayerColor::create(Color4B(200, 200, 200, 150), visibleSize.width, visibleSize.height - 640);
    layer->setPosition(Vec2(origin.x, origin.y + 640));
    this->addChild(layer);
    
    m_content = Node::create();
    m_scrollView = ScrollView::create(Size(visibleSize.width, 640), m_content);
    m_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_scrollView->setDelegate(this);
    m_scrollView->setLocalZOrder(kPageScrollViewZOrder);
    this->addChild(m_scrollView);
    
    
    
    auto _title = Label::createWithBMFont(
                                          "NFTitle.fnt",
                                          MYMultiLanguageManager::getInstance()->getText("e_editorTitle")
                                          );
    _title->setAnchorPoint(Vec2(0.5, 0.5));
    _title->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 30 - (visibleSize.height - 640)/4));
    _title->setOpacity(100);
    _title->setScale(0.8f);
    this->addChild(_title);
    
    
//    m_curPage = EditorPage::create();
//    m_curPage->setEditorListener(this);
//    m_content->addChild(m_curPage);
//    m_scrollView->setContentSize(m_curPage->getContentSize());
    
    //m_scrollView->setViewSize(Size(visibleSize.width - 200, visibleSize.height));
    
    m_firstMenuContainer = EditorScrollMenu::create();
    m_firstMenuContainer->setAnchorPoint(Vec2(0, 0));
    m_firstMenuContainer->setPosition(Vec2(origin.x + visibleSize.width, origin.y));
    m_firstMenuContainer->setLocalZOrder(kFirstMenuScrollViewZOrder);
    this->addChild(m_firstMenuContainer);
    
    m_secondMenuContainer = EditorScrollMenu::create();
    m_secondMenuContainer->setAnchorPoint(Vec2(0, 0));
    m_secondMenuContainer->setPosition(Vec2(origin.x + visibleSize.width, origin.y));
    m_secondMenuContainer->setLocalZOrder(kSecondMenuScrollViewZOrder);
    this->addChild(m_secondMenuContainer);
    
    m_thirdMenuContainer = EditorScrollMenu::create();
    m_thirdMenuContainer->setAnchorPoint(Vec2(0, 0));
    m_thirdMenuContainer->setPosition(Vec2(origin.x + visibleSize.width, origin.y));
    m_thirdMenuContainer->setLocalZOrder(kThirdMenuScrollViewZOrder);
    this->addChild(m_thirdMenuContainer);
    
    auto savaFilePath = FileUtils::getInstance()->getWritablePath();
    savaFilePath.append("D_Editor/");
    if (access(savaFilePath.c_str(), F_OK) != 0)
    {
        if (mkdir(savaFilePath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP) != 0)
        {
            this->showAlter("ERROR: mkdir xx_Editor failed!");
            
        }
    }
    savaFilePath.append(kEditorTempFile);
    if (access(savaFilePath.c_str(), F_OK) != 0)
    {
        FILE* fp = fopen(savaFilePath.c_str(), "wt");
        if (!fp)
        {
            this->showAlter("ERROR: create temp file failed!");
        }
        else
        {
            auto data = new string();
            data->append("{\"objects\":[], \"events\":[], \"collections\":[]}");
            fwrite(data->c_str(), strlen(data->c_str()), 1, fp);
            fclose(fp);
            delete data;
        }
        
    }
    
    this->openFile(kEditorTempFile);
    
    return true;
}

Scene* EditorScene::createScene()
{
    auto scene = MYScene::create();
    auto layer = EditorScene::create();
    scene->addChild(layer);
    return scene;
}

void EditorScene::setFileName(const string& filename)
{
    if (m_fileName)
    {
        delete m_fileName;
        m_fileName = nullptr;
    }
    
    m_fileName = new string(filename);
    
}
void EditorScene::setFileData(const string& filename)
{
    if (m_fileData)
    {
        delete m_fileData;
        m_fileData = nullptr;
    }
    
    m_fileData = new string(filename);
}

void EditorScene::showAlter(const string& msg)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto alter = EditorText::create(msg, Size(600, 100), 40);
    alter->setAnchorPoint(Vec2(0.5, 0.5));
    alter->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    alter->setZOrder(kAlterPopViewZOrder);
    this->addChild(alter);
    alter->setScale(0.5);
    
    auto scaleTo = ScaleTo::create(0.2, 1);
    auto delayTime = DelayTime::create(0.2);
    auto fadeOut = FadeOut::create(0.2);
    auto call = CallFunc::create(CC_CALLBACK_0(EditorScene::showAlterDone, this, alter));
    alter->runAction(Sequence::create(scaleTo, delayTime, fadeOut, call, NULL));
}
void EditorScene::showAlterDone(Node* alter)
{
    alter->removeFromParentAndCleanup(true);
}

#pragma mark -  data

void EditorScene::setMission(Mission* mission)
{
    CC_SAFE_RELEASE_NULL(m_mission);
    m_mission = mission;
    CC_SAFE_RETAIN(m_mission);
}
Mission* EditorScene::getMission()
{
    return m_mission;
}

#pragma mark - editor listener
EditorMenu* EditorScene::getMenu(MenuState state)
{
    EditorMenu* menu = nullptr;
    switch (state)
    {
        case MenuState::FIRST:
        {
            menu = m_firstMenuContainer->getMenu();
        }
            break;
        case MenuState::SECOND:
        {
            menu = m_secondMenuContainer->getMenu();
        }
            break;
        case MenuState::THIRD:
        {
            menu = m_thirdMenuContainer->getMenu();
        }
            break;
        default:
            break;
    }
    
    return menu;
}

void EditorScene::showMenu(MenuState state, EditorMenu* menu)
{
    menu->setEditorListener(this);
    
    switch (state)
    {
        case MenuState::FIRST:
        {
            this->_hideMenu(m_secondMenuContainer);
            this->_hideMenu(m_thirdMenuContainer);
            
            if (m_state == MenuState::FIRST)
            {
                auto _menu = m_firstMenuContainer->getMenu();
                
                
                if (_menu->getMenuType() != menu->getMenuType())
                {
                    this->_hideMenu(m_firstMenuContainer);
                }
                else
                {
                    return;
                }
            }
            m_firstMenuContainer->setMenu(menu);
            this->_showMenu(m_firstMenuContainer);
            
            m_state = MenuState::FIRST;
        }
            break;
        case MenuState::SECOND:
        {
            if (m_state == MenuState::NONE)
            {
                CCASSERT(0, "MENU ERROR!");
            }
            this->_hideMenu(m_thirdMenuContainer);
            if (m_state == MenuState::SECOND)
            {
                auto _menu = m_secondMenuContainer->getMenu();
                if (_menu->getMenuType() != menu->getMenuType())
                {
                    this->_hideMenu(m_secondMenuContainer);
                }
                else
                {
                    return;
                }
            }
            
            m_secondMenuContainer->setMenu(menu);
            this->_showMenu(m_secondMenuContainer);
            
            m_state = MenuState::SECOND;
        } break;
            
        case MenuState::THIRD:
        {
            if (m_state == MenuState::NONE || m_state == MenuState::FIRST)
            {
                CCASSERT(0, "MENU ERROR!");
            }
            if (m_state == MenuState::THIRD)
            {
                auto _menu = m_thirdMenuContainer->getMenu();
                if (_menu->getMenuType() != menu->getMenuType())
                {
                    this->_hideMenu(m_thirdMenuContainer);
                }
                else
                {
                    return;
                }
            }
            
            m_thirdMenuContainer->setMenu(menu);
            this->_showMenu(m_thirdMenuContainer);
            
            m_state = MenuState::THIRD;
        } break;
            
        default:
            break;
    }
    
    this->layoutScene();
}

void EditorScene::hideMenu(MenuState state)
{
    switch (state)
    {
        case MenuState::FIRST:
        {
            this->_hideMenu(m_firstMenuContainer);
            this->_hideMenu(m_secondMenuContainer);
            this->_hideMenu(m_thirdMenuContainer);
            m_state = MenuState::NONE;
        }
            break;
        case MenuState::SECOND:
        {
            this->_hideMenu(m_secondMenuContainer);
            this->_hideMenu(m_thirdMenuContainer);
            m_state = MenuState::FIRST;
        }
            break;
        case MenuState::THIRD:
        {
            this->_hideMenu(m_thirdMenuContainer);
            m_state = MenuState::SECOND;
        }
            break;
        default:
            break;
    }
    this->layoutScene();
}

void EditorScene::addObject(EditorPhysicNodeContainer* obj)
{
    auto offset = m_scrollView->getContentOffset();
//    auto viewSize = m_scrollView->getViewSize();
    auto pos = Vec2(-offset.x, -offset.y);
    obj->setPosition(pos);
    obj->setEditorListener(this);
    m_curPage->addObject(obj);
}
void EditorScene::addEvent(EditorPhysicNodeContainer* evt)
{
    
}
void EditorScene::addCollection(EditorPhysicNodeContainer* col, Collection* collection)
{
    auto offset = m_scrollView->getContentOffset();
    //    auto viewSize = m_scrollView->getViewSize();
    auto pos = Vec2(-offset.x, -offset.y);
    col->setPosition(pos);
    col->setEditorListener(this);
    col->setEditorListener(this);
    m_curPage->addCollection(col, collection);
}
void EditorScene::removeEditorPhysicNodeContainer(EditorPhysicNodeContainer* cor)
{
    m_curPage->removeEditorPhysicNodeContainer(cor);
    this->hideMenu(EditorListener::MenuState::FIRST);
}
void EditorScene::clearUpCurrentPage()
{
    m_curPage->clearPage();
}
void EditorScene::openFile(const std::string& filePath)
{
    if ("" == filePath || " " == filePath)
    {
        this->showAlter("Please input file name");
        return;
    }
    string data = string();
    
    bool isExist = false;
    if (FileUtils::getInstance()->isFileExist(filePath))
    {
        
        data = FileUtils::getInstance()->getStringFromFile(filePath);
        //log("FILEPATH: %s\n", FileUtils::getInstance()->fullPathForFilename(filePath).c_str());
        
        this->setFileName(FileUtils::getInstance()->fullPathForFilename(filePath));
        isExist = true;
    }
    else
    {
        auto doc = FileUtils::getInstance()->getWritablePath();
        doc.append("D_Editor/").append(filePath);
        //log("FILEPATH: %s\n", doc.c_str());
        if (access(doc.c_str(), F_OK) == 0)
        {
            this->setFileName(doc);
            
            unsigned char* buffer = nullptr;
            ssize_t size = 0;
            size_t readsize;
           
            FILE *fp = fopen(doc.c_str(), "rt");
            if (!fp)
            {
                isExist = false;
            }
            fseek(fp,0,SEEK_END);
            size = ftell(fp);
            fseek(fp,0,SEEK_SET);
            
            buffer = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
            buffer[size] = '\0';
            
            readsize = fread(buffer, sizeof(unsigned char), size, fp);
            fclose(fp);
            
            
            if (nullptr == buffer || 0 == readsize)
            {
                std::string msg = "Get data from file(";
                msg.append(filePath).append(") failed!");
                this->showAlter(msg);
            }
            else
            {
                data = string((const char*)buffer);
            }
            
            isExist = true;
        }
    }
    
    //log("Load Data : %s", data.c_str());
    
    if (isExist)
    {
        
        this->setFileData(data);
        //封装成Mission
        auto missionData = string();
        missionData.append("{");
        missionData.append("\"pages\":[");
        missionData.append(data);
        missionData.append("]}");
        
        rapidjson::Document doc;
        doc.Parse<0>(missionData.c_str());
        if (doc.HasParseError())
        {
            CCASSERT(false, "json parse error");
        }
        //clear last pages
        for (int i = 0; i < m_editorPages->count(); ++ i)
        {
            auto page = dynamic_cast<EditorPage*>(m_editorPages->getObjectAtIndex(i));
            page->removeFromParentAndCleanup(true);
        }
        m_editorPages->removeAllObjects();

        auto mission = Mission::createWithDataStr(missionData);
        this->setMission(mission);

        for (int i = 0; i < m_mission->getMissionPageCount(); ++ i)
        {
            auto missionPage = mission->getMissionPage(i);
            auto page = EditorPage::create();
            page->setEditorListener(this);
            page->loadMissionPage(missionPage);
            m_content->addChild(page);
            m_editorPages->addObject(page);
        }
        this->showPage(0);
        this->hideMenu(EditorListener::MenuState::FIRST);
    }
    else
    {
        this->showAlter("FILE IS NOT EXIST!");
    }
    
}
void EditorScene::save(const std::string& filePath)
{
    string savaFilePath = string();
    
    //如果输入了文件名则 覆盖或新建文件
    if (filePath != "" && filePath != " ")
    {
        savaFilePath = FileUtils::getInstance()->getWritablePath();
        savaFilePath.append("D_Editor/");
        savaFilePath.append(filePath);
    }
    //保存到打开的文件
    else if(this->getFileName())
    {
        if (access(this->getFileName()->c_str(), W_OK) == 0)
        {
            savaFilePath = string(this->getFileName()->c_str());
        }
        else
        {
            string str = string(this->getFileName()->c_str());
            str.append(" CAN'T BE WRITE!");
            this->showAlter(str);
            return;
        }
    }
    //都没有的情况下就提示输入文件名
    else
    {
        this->showAlter("Please input file name");
        return;
    }
    
    //log("SAVE FILE PATH: %s", savaFilePath.c_str());
    
    FILE* fp = fopen(savaFilePath.c_str(), "wt");
    if (fp)
    {
        auto data = new string();
        for (int i = 0; i < m_editorPages->count(); ++ i)
        {
            auto page = dynamic_cast<EditorPage*>(m_editorPages->getObjectAtIndex(i));
            page->save(data);
            if (i != m_editorPages->count()-1)
            {
                data->append(",");
            }
        }
        fwrite(data->c_str(), strlen(data->c_str()), 1, fp);
        fclose(fp);
        //log("SAVE: %s", data->c_str());
        delete data;
        this->showAlter(MYMultiLanguageManager::getInstance()->getText("e_alertSaveSuccess"));
    }
    else
    {
        string str = string(filePath.c_str());
        str.append(" CAN'T BE SAVE!");
        this->showAlter(str);
        return;
    }
    

//    int r = access(savaFilePath.c_str(), W_OK);
//    log("WRITE : %d", r);
//    
//    r = access(savaFilePath.c_str(), R_OK);
//    log("READ : %d", r);
}

void EditorScene::run()
{
    //default save
    this->save("");
    auto dataStr = new string();
    dataStr->append("{\"pages\":[");
    for (int i = 0; i < m_editorPages->count(); ++ i)
    {
        auto page = dynamic_cast<EditorPage*>(m_editorPages->getObjectAtIndex(i));
        page->save(dataStr);
        if (i != m_editorPages->count()-1)
        {
            dataStr->append(",");
        }
    }
    dataStr->append("]}");
    auto mission = Mission::createWithDataStr(*dataStr);
    delete dataStr;
    mission->setMissionRepeatModel(Mission::MissionRepeatModel::ALL);
    auto cr = CoolRun::createScene(mission, CoolRun::RunType::EDITOR);
    SceneHelp::replaceScene(cr);
}

int EditorScene::pageNumber()
{
    return (int)m_editorPages->count();
}

void EditorScene::showPage(int index)
{
    CCASSERT(index >=0 && index < m_editorPages->count(), "page index is out of range");
    for (int i = 0; i < m_editorPages->count(); ++ i)
    {
        auto page = dynamic_cast<EditorPage*>(m_editorPages->getObjectAtIndex(i));
        if (i == index)
        {
            if (!page->isVisible())
            {
                page->setVisible(true);
            }
            
            m_curPage = page;
            m_content->setContentSize(m_curPage->getContentSize());
        }
        else
        {
            if (page->isVisible())
            {
                page->setVisible(false);
            }
        }
    }
}

void EditorScene::addPage()
{
    auto page = EditorPage::create();
    page->setEditorListener(this);
    m_content->addChild(page);
    m_editorPages->addObject(page);
    this->showPage((int)m_editorPages->count()-1);
}

void EditorScene::deletePage(int index)
{
    for (int i = 0; i < m_editorPages->count(); ++ i)
    {
        if (i == index)
        {
            auto page = dynamic_cast<EditorPage*>(m_editorPages->getObjectAtIndex(i));
            page->removeFromParentAndCleanup(true);
            m_editorPages->removeObject(page);
            break;
        }
    }
    if (m_editorPages->count() > 0)
    {
        this->showPage(index-1);
    }
}

#pragma mark - help

void EditorScene::layoutScene()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    auto menuSize = m_menuContainer->getContentSize();
//    m_scrollView->setViewSize(Size(visibleSize.width - menuSize.width, visibleSize.height));
    switch (m_state)
    {
        case MenuState::NONE:
        {
            m_scrollView->setViewSize(Size(visibleSize.width, 640));
        }
            break;
        case MenuState::FIRST:
        {
            auto pos = m_firstMenuContainer->getPosition();
            m_scrollView->setViewSize(Size(pos.x, 640));
        }
            break;
        case MenuState::SECOND:
        {
            auto pos = m_secondMenuContainer->getPosition();
            m_scrollView->setViewSize(Size(pos.x, 640));
        }
            break;
        case MenuState::THIRD:
        {
            auto pos = m_thirdMenuContainer->getPosition();
            m_scrollView->setViewSize(Size(pos.x, 640));
        }
            break;
            
        default:
            break;
    }
}

void EditorScene::_hideMenu(EditorScrollMenu* container)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    container->setPosition(Vec2(origin.x + visibleSize.width, origin.y));
    container->setMenu(EditorMenu::create());
}
void EditorScene::_showMenu(EditorScrollMenu* container)
{
    if (container == m_firstMenuContainer)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        auto csize = m_firstMenuContainer->getContentSize();
        auto pos = m_firstMenuContainer->getPosition();
        container->setPosition(Vec2(origin.x + visibleSize.width - csize.width, pos.y));
    }
    else if (container == m_secondMenuContainer)
    {
        auto csize = m_secondMenuContainer->getContentSize();
        auto pos = m_firstMenuContainer->getPosition();
        container->setPosition(Vec2(pos.x - csize.width, pos.y));
    }
    else if (container == m_thirdMenuContainer)
    {
        auto csize = m_thirdMenuContainer->getContentSize();
        auto pos = m_secondMenuContainer->getPosition();
        container->setPosition(Vec2(pos.x - csize.width, pos.y));
    }
}

