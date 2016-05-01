#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "QuadTree.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer 
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    //game sence callback
    void onEnter();
    void onExit();
    
    //touch callback
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchMoved(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void ccTouchCancelled(CCTouch* touch, CCEvent* event);
    
private:
    int mapID;                                                              //ID of the map
    bool dragingResource;                                                   //is dragging resource?
    int dragTag;                                                            //dragging resource tag
    int currentResourceTag;                                                 //current added resource tag
    int currentItemTag;                                                     //tag for item to be added
    CCArray * itemArray;                                                    //item(added on mao) array
    CCArray * resourceArray;                                                //resource array
    CCSprite * delSprite;                                                   //garbage can sprite
    char * serverIp;                                                        //server ip
    rapidjson::Value userDataArray(rapidjson::kArrayType);                  //array to collect user data
    QuadTree * qTree;                                                       //quadtree
    CCRect screenRect;                                                      //current view rect

    update(float delta);                                                    //update function: called every frame
    
    void addNewSpriteWithCoords(CCPoint p, int tag);                        //add new sprite
    void loadTerrain(int type);                                             //load terrain
    void loadFrame(int type);                                               //load game UI frame
    void loadResourcese();                                                  //load map resource
    
    void addUserData(char * dataType,char * resource);                      //add user data
    
    bool containsTouchLocation(cocos2d::CCTouch *touch);
    
    void synchronizeMapRequest();                                           //synchronize the map with server
    void onSynchronizeMapCompleted(cocos2d::CCNode *sender ,void *data);    //call when succssful synchronize the map
    void updateMapRequest();                                                //update the map from server
    void onUpdateMapCompleted(cocos2d::CCNode *sender ,void *data);         //call when succssful update the map
    void changeItemRequest(int oprateType);                                 //change item on server
    void onChangeItemCompleted(cocos2d::CCNode *sender ,void *data);        //call when successful change the item
    void uploadUserDataRequest();                             //upload user experience data to the server
    void onUploadUserDataCompleted(cocos2d::CCNode *sender ,void *data);    //call when successful upload user experience data

    void shiftUpCallback(CCObject* pSender);                                //shift map up
    void shiftDownCallback(CCObject* pSender);                              //shift map down
    void shiftLeftCallback(CCObject* pSender);                              //shift map left
    void shiftRightCallback(CCObject* pSender);                             //shift map right
    void updateMap(CCRect rect, int offsetx, int offsety);                  //update map after shifting
};

#endif // __HELLOWORLD_SCENE_H__
