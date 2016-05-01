//
//  QuadTree.h
//


#ifndef __MapOptimiz__QuadTree__
#define __MapOptimiz__QuadTree__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

#endif /* defined(__MapOptimiz__QuadTree__) */

#ifndef __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__
#define __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__


/* quadrant of the rect
 
 second quadrant   |    first quadrant
 ------------------|-------------------
 third quadrant    |    fourth quadrant
 */
typedef enum
{
    firstQ = 0,
    secondQ = 1,
    thirdQ,
    fourthQ
}QuadrantEnum;

//rect structure
typedef struct quadrect
{
    double  left,
    top,
    right,
    bottom;
}quadRect;

//structure of quadtree node
typedef struct quadnode
{
    quadRect    rect;               //rect of the node
    CCSprite    *object;            //object store in the node
    struct  quadnode  *sub[4];      //4 child nodes of the node
}quadNode;

//structure of quadtree
typedef struct quadtree
{
    quadNode    *root;              //root of the quadtree
    int         depth;              //depth of the quadtree
}quadTree;

class QuadTree {
    
public:
    //build the quadtree
    void insertObject(CCSprite * object);
    
    //search the quadtree
    CCSprite * findObjectAtPoint(CCPoint pos);
    CCArray * findObjectInRect(CCRect rect);
    
private:
    quadTree * qTree;
    CCArray * objectArray;              //array to store objects in given rect
    
    void insert(CCSprite * object, quadNode *node);
    
    CCSprite * find(quadNode *node, CCPoint pos);
    
    bool nodeHasChild(quadNode *node);
    
    int getQuadrant(CCPoint p, quadRect& rect);
    int getQuadrant(CCSprite * object, quadRect& rect);

    void findInRect(quadNode *node, quadRect& rect);
};


#endif