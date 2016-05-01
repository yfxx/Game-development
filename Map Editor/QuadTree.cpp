//
//  QuadTree.cpp
//

#include "QuadTree.h"

//interface for CC Class
void QuadTree::insertObject(CCSprite * object)
{
    insert(object, qTree->root);
}

//insert object to quardtree
//construct the tree, where objects are all at leaf nodes
void QuadTree::insert(CCSprite * object, quadNode *node)
{
    //if the node already has child, the object should be inserted in its children
    if (nodeHasChild(node))
    {
        quadRect & r = node->rect;
        if (getQuadrant(object, r) >= 0) {
            insert(object, node->sub[getQuadrant(object, r)]);
        }
    }
    
    //if the node has no child and it has object
    else if (node->object)
    {
        //create 4 child nodes
        for (int i = 0 ; i < 4 ; i++)
            node->sub[i] = new quadNode;
        
        //put the old object to the the child node accroding to the quatrant
        CCSprite * oldObject = node->object;
        node->object = NULL;
        
        quadRect & r = node->rect;
        if (getQuadrant(oldObject, r) >= 0) {
            node->sub[getQuadrant(oldObject, r)]->object = object;
            
            //insert the object to its childern
            insert(object, node->sub[getQuadrant(object, r)]);
        }
    }
    //if the node has no child and doesn't has object, put object here
    else
        node->object = object;
}


//whether a node has child nodes
bool QuadTree::nodeHasChild(quadNode *node)
{
    for (int i = 0; i < 4; i++)
    {
        if (node->sub[i])
            return true;
    }
    return false;
}

//get the quadrant of the object
/* quadrant of the rect
 
 second quadrant   |    first quadrant
 ------------------|-------------------
 third quadrant    |    fourth quadrant
 */
int QuadTree::getQuadrant(CCPoint p, quadRect& rect)
{
    //distance to each edges
    int leftDist = p.x - rect.left;
    int rightDist = rect.right - p.x;
    int topDist = rect.top - p.y;
    int bottomDist = p.y - rect.bottom;
    
    if (p.x < rect.left || p.x > rect.right || p.y < rect.bottom || p.y > rect.top) {
        return -1;
    }
    if (leftDist > rightDist)
    {
        if (topDist > bottomDist)
            return 3;
        else
            return 0;
    }
    else
    {
        if (topDist > bottomDist)
            return 2;
        else
            return 1;
    }
    
}

//get the quadrant of the object
int QuadTree::getQuadrant(CCSprite * object, quadRect& rect)
{
    CCPoint p = object->getPosition();
    return getQuadrant(p, rect);
}

//touch point in some object
bool pointInObject(CCSprite * object, CCPoint p)
{
    if (object->boundingBox().containsPoint(p))
        return true;
    else
        return false;
}

//rect has intersection with rect
bool rectIntersectRect(quadRect& r1, quadRect& r2)
{
    if (r1.bottom > r2.top || r1.top < r2.bottom || r1.right < r2.left || r1.left > r2.right)
        return false;
    
    return true;
}

//interface for CC Class
CCSprite * QuadTree::findObjectAtPoint(CCPoint pos)
{
    return find(qTree->root, pos);
}

//interface for CC Class
CCArray * QuadTree::findObjectInRect(CCRect rect)
{
    objectArray = CCArray::create();
    objectArray->retain();
    
    quadRect r = {rect.getMinX(), rect.getMaxY(), rect.getMaxX(), rect.getMinY()};
    quadRect & _r = r;
    findInRect(qTree->root, _r);
    return objectArray;
}

//find the target object with given position
CCSprite * QuadTree::find(quadNode *node, CCPoint pos)
{
    if (pointInObject(node->object, pos))
        return node->object;
    quadRect & r = node->rect;
    int quadrant = getQuadrant(pos, r);
    //can not find the object
    if (!nodeHasChild(node) || quadrant == -1) {
        return 0;
    }
    return find(node->sub[quadrant], pos);
}

//find all objects in the given rect
void QuadTree::findInRect(quadNode *node, quadRect& rect)
{
    //if rect has intersection with node's rect
    if (rectIntersectRect(node->rect, rect)) {
        //if node has no child
        if (!nodeHasChild(node)) {
            if (node->object)
                objectArray->addObject(node->object);
        }
        //find in sub rects
        else if(rectIntersectRect(node->sub[firstQ]->rect, rect))
            findInRect(node->sub[firstQ], rect);
        else if(rectIntersectRect(node->sub[secondQ]->rect, rect))
            findInRect(node->sub[secondQ], rect);
        else if(rectIntersectRect(node->sub[thirdQ]->rect, rect))
            findInRect(node->sub[thirdQ], rect);
        else if(rectIntersectRect(node->sub[fourthQ]->rect, rect))
            findInRect(node->sub[fourthQ], rect);
    }
}
