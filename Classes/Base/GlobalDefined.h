//
//  GlobalDefined.h
//  CoolRun
//
//  Created by ManYou on 14-8-8.
//
//

#ifndef CoolRun_GlobalDefined_h
#define CoolRun_GlobalDefined_h

#define MY_RELEASE  0
#define COCOS2D_DEBUG -1

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)

#define SHOW_ORIGIN_AND_CONTENT this->showOriginDot(); this->showContentSizeRect()
#define SHOW_RECT(x) this->showRect((x))

#else

#define SHOW_ORIGIN_AND_CONTENT
#define SHOW_RECT(x) 

#endif

#define PRINTRECT(rect) log("(%f, %f, %f, %f)", (rect).origin.x, (rect).origin.y, (rect).size.width, (rect).size.height)

#define EXCHANGE_PHYSICNODE_XV_XA(node1, node2) auto tV = (node1)->getXV();       \
                                                (node1)->setXV((node2)->getXV());   \
                                                (node2)->setXV(tV);               \
                                                auto aV = (node1)->getXA();       \
                                                (node1)->setXA((node2)->getXA());   \
                                                (node2)->setXA(aV)

#define EXCHANGE_PHYSICNODE_YV_YA(node1, node2) auto tV = (node1)->getYV();           \
                                                (node1)->setYV((node2)->getYV());       \
                                                (node2)->setYV(tV);                   \
                                                auto aV = (node1)->getYA();           \
                                                (node1)->setYA((node2)->getYA());       \
                                                (node2)->setYA(aV)
#define CREATEANDRETAINARRAY(x) (x) = __Array::create(); CC_SAFE_RETAIN((x))

#define SAFEREMOVECHILD(x) if(x) { (x)->removeFromParent(); }
#define SAFEREMOVEANDCLEARNCHILD(x) if(x) { (x)->removeFromParentAndCleanup(true); }

#define SAFESETVISIBLE(x, y) if (x) { if((x)->isVisible() != (y)) { (x)->setVisible((y)); } }


#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)


#endif
