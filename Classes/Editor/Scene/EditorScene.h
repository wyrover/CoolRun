//
//  EditorScene.h
//  CoolRun
//
//  Created by ManYou on 15/1/28.
//
//

#ifndef __CoolRun__EditorScene__
#define __CoolRun__EditorScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "EditorListener.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class EditorScrollMenu;
class EditorMainMenu;
class EditorMenu;
class EditorPage;
class Mission;
class EditorScene : public Layer, public ScrollViewDelegate, public EditorListener
{
public:
    EditorScene();
    ~EditorScene();
    
    virtual bool init() override;
    CREATE_FUNC(EditorScene);
    
    static Scene* createScene();
    
    void setFileName(const string& filename);
    string* getFileName() { return m_fileName; }
    void setFileData(const string& filename);
    string* getFileData() { return m_fileData; }
    
    void showAlter(const string& msg);
    void showAlterDone(Node* alter);
    
    //Mission
    void setMission(Mission* mission);
    Mission* getMission();
    
    //scrollView
    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {};
    
    //EditorListener
    virtual void showMenu(MenuState state, EditorMenu* menu) override;
    virtual void hideMenu(MenuState state) override;
    virtual EditorMenu* getMenu(MenuState state) override;
    virtual void addObject(EditorPhysicNodeContainer* obj) override;
    virtual void addEvent(EditorPhysicNodeContainer* evt) override;
    virtual void addCollection(EditorPhysicNodeContainer* col, Collection* collection) override;
    virtual void removeEditorPhysicNodeContainer(EditorPhysicNodeContainer* cor) override;
    virtual void clearUpCurrentPage() override;
    virtual void openFile(const std::string& filePath) override;
    virtual void save(const std::string& filePath) override;
    virtual void run() override;
    virtual int pageNumber() override;
    virtual void showPage(int index) override;
    virtual void addPage() override;
    virtual void deletePage(int index) override;
private:
    void layoutScene();
    void _hideMenu(EditorScrollMenu* container);
    void _showMenu(EditorScrollMenu* container);
private:
    
    ScrollView* m_scrollView;
    Node* m_content;
    EditorPage* m_curPage;
    
    __Array* m_editorPages;
    
    Mission* m_mission;
    string* m_fileData;
    
    EditorScrollMenu* m_firstMenuContainer;
    EditorScrollMenu* m_secondMenuContainer;
    EditorScrollMenu* m_thirdMenuContainer;
    
    MenuState m_state;
    string* m_fileName;
};

#endif /* defined(__CoolRun__EditorScene__) */
