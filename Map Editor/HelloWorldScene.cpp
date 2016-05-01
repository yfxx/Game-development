//
//  HelloWorldScene.cpp
//

#include "HelloWorldScene.h"

USING_NS_CC;

//item tag enum
enum Tag{
    bgTag = 0,
    lableTag = 1,
    terrainTag,
    frame1Tag,
    resourceTag,
    delBtnTag,
    newRescorceTag = 500,
};

//z value enum
enum Z{
    bgZ = 0,
    terrainZ = 1,
    resourceZ,
    newResourceZ,
    labelZ,
};

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + visibleSize.width - 140, origin.y + 140);

    screenRect = CCRect(origin.x, origin.y, visibleSize.width, visibleSize.height);
    
    CCMenuItemImage *pUpItem = CCMenuItemImage::create(
                                                       "up.png",
                                                       "up.png",
                                                       this,
                                                       menu_selector(HelloWorld::shiftUpCallback));
    pUpItem->setPosition(ccp(center.x, center.y + pUpItem->getContentSize().height/2 + 20));
    
    CCMenuItemImage *pDownItem = CCMenuItemImage::create(
                                                         "down.png",
                                                         "down.png",
                                                         this,
                                                         menu_selector(HelloWorld::shiftDownCallback));
    pDownItem->setPosition(ccp(center.x, center.y - pDownItem->getContentSize().height/2 - 20));
    
    CCMenuItemImage *pLeftItem = CCMenuItemImage::create(
                                                         "left.png",
                                                         "left.png",
                                                         this,
                                                         menu_selector(HelloWorld::shiftLeftCallback));
    pLeftItem->setPosition(ccp(center.x - pLeftItem->getContentSize().width/2 - 20 ,center.y));
    
    CCMenuItemImage *pRightItem = CCMenuItemImage::create(
                                                          "right.png",
                                                          "right.png",
                                                          this,
                                                          menu_selector(HelloWorld::shiftRightCallback));
    pRightItem->setPosition(ccp(center.x + pRightItem->getContentSize().width/2 + 20 , center.y));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pUpItem,pDownItem,pLeftItem,pRightItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    
    serverIp = "http://127.0.0.1:1337/";
    currentItemTag = 1000;
    currentResourceTag = newRescorceTag;                                        //current new resource tag
    dragingResource = false;                                                    //is dragging resource
    
    itemArray = CCArray::create();                                              //item array to store created resources
    itemArray->retain();
    
    qTree = new QuadTree();
    
    //load game elements
    this->loadTerrain(1);
    this->loadFrame(1);
    this->loadResourcese();
    
    //synchronize the map
    this->synchronizeMapRequest();
    
    this->scheduleUpdate();
    
    return true;
}

#pragma mark - init
void HelloWorld::loadTerrain(int type){
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("terrain_1.BMP", 50);
    this->addChild(batchNode, 0, terrainTag);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            this->addNewSpriteWithCoords( ccp(-1280 + 640*i, -1920 + 960*j),terrainTag);
        }
    }
}

//load game UI frame
void HelloWorld::loadFrame(int type){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();        //screen size
    CCPoint o = CCDirector::sharedDirector()->getVisibleOrigin();     //screen origin
    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("frame1.png", 50);
    this->addChild(batchNode, 0, frame1Tag);
    for (int i = 0; i < s.height/10; i++) {
        this->addNewSpriteWithCoords( ccp(s.width-105+53, 10*i+5),frame1Tag);
    }
    
    delSprite = CCSprite::create("del_btn.png");
    delSprite->setScale(2);
    delSprite->setPosition(ccp(s.width-130, 30));
    this->addChild(delSprite, bgZ, delBtnTag);
    
    //end button
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",this,menu_selector(HelloWorld::menuCloseCallback));
    pCloseItem->setPosition(ccp(o.x + s.width - pCloseItem->getContentSize().width/2 ,o.y + pCloseItem->getContentSize().height/2));
    
    //meun
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Drag to add new one", "Arial", 16);
    pLabel->setPosition(ccp(s.width - pLabel->getContentSize().width/2, o.y + s.height-20));
    this->addChild(pLabel, labelZ);
}

//load map resource
void HelloWorld::loadResourcese(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();                      //screen size
    
    resourceArray = CCArray::create();                                              //item array to store created resources
    resourceArray->retain();

    for (int i = 0; i < 10; i++) {
        CCString * string = CCString::createWithFormat("s0000%d.png",i);
        CCSprite * pResource = CCSprite::create(string->getCString());
        float height = pResource->boundingBox().getMaxY() - pResource->boundingBox().getMinY();
        float scale = 40.0f/height;
        printf("scale%f",scale);
        pResource->setScale(scale);
        pResource->setPosition(ccp(s.width-50, 50+ 60*i));
        this->addChild(pResource, resourceZ);
        resourceArray->addObject(pResource);
    }
}

void HelloWorld::addNewSpriteWithCoords(CCPoint p, int tag)
{
    CCSpriteBatchNode* BatchNode = (CCSpriteBatchNode*)getChildByTag(tag);

    CCSprite* sprite = CCSprite::createWithTexture(BatchNode->getTexture());
    BatchNode->addChild(sprite);
    sprite->setPosition( ccp( p.x, p.y) );
    
    //insert object to quadtree
    qTree->insertObject(sprite);
}

//add user experience data
void HelloWorld::addUserData(char * dataType,char * resource){
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    
    object.AddMember("dataType",dataType, allocator);
    object.AddMember("resource",resource, allocator);
    
    userDataArray.PushBack(object, allocator);
}

#pragma mark - update
//update function: call every frame
void HelloWorld::update(float delta){
    this->updateMapRequest();
}

#pragma mark - touch
bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //if tap in resource's boundary
    int i = 0;
    CCObject * obj;
    CCARRAY_FOREACH(resourceArray,obj)
    {
        CCSprite * pResource = (CCSprite*)obj;
        if(pResource->boundingBox().containsPoint(pTouch->getLocation())){
            CCString * name = CCString::createWithFormat("s0000%d.png",i);
            CCSprite * pNewResource = CCSprite::create(name->getCString());
            pNewResource->setPosition(pResource->getPosition());
            this->addChild(pNewResource,newResourceZ,currentResourceTag);
            
            itemArray->addObject(pNewResource);
            
            dragingResource = true;
            dragTag = currentResourceTag;
            
            return true;
        }
        i++;
    }
    //judge if tap in exsit game object
    //here we use quadtree to find the target object
    if (qTree->findObjectAtPoint(pTouch->getLocation())) {
        CCSprite * sprite = qTree->findObjectAtPoint(pTouch->getLocation());
        if (sprite->getTag() != terrainTag) {
            dragingResource = true;
            dragTag = sprite->getTag();
            
            return true;
        }
    }
    return true;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    if (dragingResource) {
            CCSprite * pNewResource = (CCSprite*)this->getChildByTag(dragTag);
            pNewResource->setPosition(touch->getLocation());
    }
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    if (dragingResource) {
        if (delSprite->boundingBox().containsPoint(touch->getLocation())) {
            //delete dragging item
            if (dragTag == currentResourceTag) {
                CCSprite * pNewResource = (CCSprite*)this->getChildByTag(dragTag);
                this->removeChild(pNewResource);
                
                //add user experience data
                char * resourceTag;
                sprintf(resourceTag, "%d", currentResourceTag);
                this->addUserData("giveUpWhenAddObject", resourceTag);
            }
            //delete item
            else{
                CCSprite * pNewResource = (CCSprite*)this->getChildByTag(dragTag);
                this->removeChild(pNewResource);
                itemArray->removeObject(pNewResource);
                
                //delete item from server
                this->changeItemRequest(0);
                
                //add user experience data
                char * resourceTag;
                sprintf(resourceTag, "%d", currentResourceTag);
                this->addUserData("deleteObject", resourceTag);
            }
        }
        //finish dragging
        else{
            //add item to server
            this->changeItemRequest(1);
            
            //add user experience data
            char * resourceTag;
            sprintf(resourceTag, "%d", currentResourceTag);
            this->addUserData("addObject", resourceTag);
            
            dragingResource = false;
            currentResourceTag++;
        }
    }
}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    dragingResource = false;
}

#pragma mark - system
void HelloWorld::onEnter(){
    //add touch supervise
    CCDirector * pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void HelloWorld::onExit(){
    //remove touch supervise
    CCDirector * pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    
    //upload user data when exit
    this->uploadUserDataRequest();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

#pragma mark - net
//synchronize the map with server
void HelloWorld::synchronizeMapRequest(){
    extension::CCHttpRequest* request = new extension::CCHttpRequest();
    char * url = strcat(serverIp, "/api/synchronizeMap");
    request->setUrl(url);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(HelloWorld::onSynchronizeMapCompleted));
    request->setTag("synchronizeMap");
    extension::CCHttpClient::getInstance()->send(request);
    request->release();
}

//call when succssful synchronize the map
void HelloWorld::onSynchronizeMapCompleted(cocos2d::CCNode *sender ,void *data){
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    if (!response)
        return;
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    printf("Http response,code:%d response string:%s tag:%s",statusCode,statusString,response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
        CCLog("response failed \nerror buffer:%s" ,response->getErrorBuffer());

    //parse the response json data
    std::vector<char> *buffer = response->getResponseData();
    std::string jsonStr = std::string(buffer->begin(),buffer->end());
    printf("Response jsonString:\n %s",jsonStr.c_str());
    
    //json document for rapidjson lib
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    if(doc.HasParseError())
        CCLOG("GetParseError%s\n",doc.GetParseError());
    
    rapidjson::Value&valString=doc["success"];
    if(valString.IsString()){
        //if successful
        if (strcmp(valString.GetString(), "1")) {
            rapidjson::Value&itemsArray=doc["items"];
            if(itemsArray.IsArray())
            {
                for (int i = 0 ; i < itemsArray.Capacity() ; i++) {
                    rapidjson::Value&item=itemsArray[i];
                    int x = item["xPos"].GetDouble();
                    int y = item["yPos"].GetDouble();
                    this->addNewSpriteWithCoords(ccp(x, y), currentItemTag);
                    currentItemTag++;
                }
            }
        }
    }
}

//update the map from server
void HelloWorld::updateMapRequest(){
    extension::CCHttpRequest* request = new extension::CCHttpRequest();
    char * url = strcat(serverIp, "/api/updateMap");
    request->setUrl(url);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(HelloWorld::onUpdateMapCompleted));
    request->setTag("updateMap");
    extension::CCHttpClient::getInstance()->send(request);
    request->release();
}


//call when succssful update the map
void HelloWorld::onUpdateMapCompleted(cocos2d::CCNode *sender ,void *data){
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    if (!response)
        return;
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    printf("Http response,code:%d response string:%s tag:%s",statusCode,statusString,response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
        CCLog("response failed \nerror buffer:%s" ,response->getErrorBuffer());
    
    //parse the response json data
    std::vector<char> *buffer = response->getResponseData();
    std::string jsonStr = std::string(buffer->begin(),buffer->end());
    printf("Response jsonString:\n %s",jsonStr.c_str());
    
    //json document for rapidjson lib
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    if(doc.HasParseError())
        CCLOG("GetParseError%s\n",doc.GetParseError());

    rapidjson::Value&valString=doc["success"];
    if(valString.IsString()){
        //if successful
        if (strcmp(valString.GetString(), "1")) {
            rapidjson::Value&itemsArray=doc["items"];
            if(itemsArray.IsArray())
            {
                for (int i = 0 ; i < itemsArray.Capacity() ; i++) {
                    rapidjson::Value&item=itemsArray[i];
                    
                    //oprate type 1 for add, 2 for delete
                    if (strcmp(item["oprateType"].GetString(), "1")) {
                        int x = item["xPos"].GetDouble();
                        int y = item["yPos"].GetDouble();
                        this->addNewSpriteWithCoords(ccp(x, y), currentItemTag);
                        currentItemTag++;
                    }
                    else if(strcmp(item["oprateType"].GetString(), "2")){
                        CCSprite * pNewResource = (CCSprite*)this->getChildByTag(item["tag"].GetInt());
                        this->removeChild(pNewResource);
                        itemArray->removeObject(pNewResource);
                    }
                }
            }
        }
    }
}

//change item to server, type 1 for add, 0 for delete
void HelloWorld::changeItemRequest(int oprateType){
    extension::CCHttpRequest* request = new extension::CCHttpRequest();
    char * url = strcat(serverIp, "/api/changeItem");
    request->setUrl(url);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    
    CCSprite * pNewResource = (CCSprite*)this->getChildByTag(currentResourceTag);

    //generate the post json
    rapidjson::Document document;
    document.SetObject();
    
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("xPos", pNewResource->getPositionX(), allocator);
    object.AddMember("yPos", pNewResource->getPositionY(), allocator);
    object.AddMember("z", pNewResource->getZOrder(), allocator);
    object.AddMember("tag", pNewResource->getTag(), allocator);
    object.AddMember("oprateType", oprateType, allocator);
    
    document.AddMember("item", object, allocator);
    
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    
    request->setRequestData(buffer, strlen(buffer));
    request->setResponseCallback(this, callfuncND_selector(HelloWorld::onChangeItemCompleted));
    request->setTag("changeItem");
    extension::CCHttpClient::getInstance()->send(request);
    request->release();
}


//call when successful add the new item
void HelloWorld::onChangeItemCompleted(cocos2d::CCNode *sender ,void *data){
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    if (!response)
        return;
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    printf("Http response,code:%d response string:%s tag:%s",statusCode,statusString,response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
        CCLog("response failed \nerror buffer:%s" ,response->getErrorBuffer());
    
    //parse the response json data
    std::vector<char> *buffer = response->getResponseData();
    std::string jsonStr = std::string(buffer->begin(),buffer->end());
    printf("Response jsonString:\n %s",jsonStr.c_str());
    
    //json document for rapidjson lib
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    if(doc.HasParseError())
        CCLOG("GetParseError%s\n",doc.GetParseError());
    
    rapidjson::Value&valString=doc["success"];
    if(valString.IsString()){
        //if successful
        if (strcmp(valString.GetString(), "1")) {
            rapidjson::Value&typeString=doc["oprateType"];
            //1 for add, 0 for delete
            if (strcmp(typeString.GetString(), "1")) {
                
            }
            else if (strcmp(typeString.GetString(), "0")){
                CCSprite * pNewResource = (CCSprite*)this->getChildByTag(doc["tag"].GetInt());
                this->removeChild(pNewResource);
                itemArray->removeObject(pNewResource);
            }
        }
    }

}

//upload user operation data
void HelloWorld::uploadUserDataRequest(){
    extension::CCHttpRequest* request = new extension::CCHttpRequest();
    char * url = strcat(serverIp, "/api/userData");
    request->setUrl(url);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    
    //generate the post json
    rapidjson::Document document;
    document.SetObject();
    
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    document.AddMember("userData", userDataArray, allocator);
    
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    
    request->setRequestData(buffer, strlen(buffer));
    request->setResponseCallback(this, callfuncND_selector(HelloWorld::onUploadUserDataCompleted));
    request->setTag("userData");
    extension::CCHttpClient::getInstance()->send(request);
    request->release();

}

void HelloWorld::onUploadUserDataCompleted(cocos2d::CCNode *sender ,void *data){
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    if (!response)
        return;
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    printf("Http response,code:%d response string:%s tag:%s",statusCode,statusString,response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
        CCLog("response failed \nerror buffer:%s" ,response->getErrorBuffer());
    
    //parse the response json data
    std::vector<char> *buffer = response->getResponseData();
    std::string jsonStr = std::string(buffer->begin(),buffer->end());
    printf("Response jsonString:\n %s",jsonStr.c_str());
    
    //json document for rapidjson lib
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    if(doc.HasParseError())
        CCLOG("GetParseError%s\n",doc.GetParseError());
    
    rapidjson::Value&valString=doc["success"];
    if(valString.IsString()){
        //if successful
        if (strcmp(valString.GetString(), "1")) {
            
        }
    }
}

//move map
void HelloWorld::shiftUpCallback(cocos2d::CCObject *pSender)
{
    screenRect.origin.y += 5;
    updateMap(screenRect, 0, 5);
}

void HelloWorld::shiftDownCallback(cocos2d::CCObject *pSender)
{
    screenRect.origin.y -= 5;
    updateMap(screenRect, 0, -5);
}

void HelloWorld::shiftLeftCallback(cocos2d::CCObject *pSender)
{
    screenRect.origin.x -= 5;
    updateMap(screenRect, -5, 0);
}

void HelloWorld::shiftRightCallback(cocos2d::CCObject *pSender)
{
    screenRect.origin.x += 5;
    updateMap(screenRect, 5, 0);
}

//update map when view moves
void HelloWorld::updateMap(CCRect rect, int offsetx, int offsety)
{
    //by using the quadtree, we can find objects in screen faster
    CCArray * array = qTree->findObjectInRect(rect);
    CCObject * obj;
    CCARRAY_FOREACH(array, obj)
    {
        CCSprite * sprite = (CCSprite*)obj;
        CCPoint p = sprite->getPosition();
        sprite->cocos2d::CCNode::setPosition(p.x + offsetx, p.y + offsety);
    }
}

