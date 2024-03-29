#ifndef __ANIPANG_SCENE_H__
#define __ANIPANG_SCENE_H__

#include "cocos2d.h"

#define ANIPANGINITVALUE 30
#define ANIPANGDISTANCE	60
#define ANIPANGSIZE	63
#define ANIPANGNUM 7

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define X 0
#define Y 1

#define DELAYTIME 0.2
using namespace std;
using namespace cocos2d;

struct CoorTool {
	int depth;
	int left;
	int right;
	int surface;
	int overfall;
};

struct Anipang {
	Sprite* anipangIcon;
	int type;
};

class ANIPANG : public Scene
{
public:
	vector<string> iconName = {
		"icon/bone.png", //0
		"icon/greenapple.png",//1
		"icon/butterfly.png",//2
		"icon/dino.png", //3
		"icon/herb.png", //4
		"icon/choucream.png", //5
		"icon/popping.png", //6
	};

	vector<pair<int, int>> explodingIcon;
	Anipang field[ANIPANGNUM][ANIPANGNUM];
	Vec2 BeganTouch;
	bool TfuncOperate;
	int NowX, NowY;
	int explodingConditions[2] = { 0, };
	bool Acted = false;
	int width = ANIPANGINITVALUE;
	int height = ANIPANGINITVALUE;

	Vec2 decide;
public:	
	void setLis();
	void reset(bool a[ANIPANGNUM][ANIPANGNUM])
	{
		for (size_t i = 0; i < ANIPANGNUM; i++)
		{
			for (size_t s = 0; s < ANIPANGNUM; s++)
			{
				a[i][s] = false;
			}
		}
	}

	void IconFall(Sprite* AnipangIcon)
	{
		DelayTime* delay = DelayTime::create(DELAYTIME);
		MoveBy* Drop = MoveBy::create(0.3, Point(0, -ANIPANGDISTANCE));
		Blink* IconBlink = Blink::create(60, 120);
		Sequence* IconDrop = Sequence::create(delay, Drop, nullptr);
		AnipangIcon->runAction(IconDrop);
	}

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void onExit();
	void moveIcon(Vec2 MovingDirection, int PorM);
	
public:
	int SearchDirection;
	int aroundX[4] = {1, -1, 0, 0};
	int aroundY[4] = {0, 0, 1, -1};
	bool visited[ANIPANGNUM][ANIPANGNUM];
	void allSearchDel();
	void delIcon();
	void explodeIcon(int first, int second, bool fallCheck);
	bool matchSearch(int targetX, int targetY, int direction, Vec2 decide);
	void resetSearch(int Ytrigger);
	bool isX = true;
	void fallingIconDel(CoorTool need);
	CoorTool needFallDelSearch(vector<pair<int, int>> target);

	void vecClear()
	{
		explodingIcon.clear();
		explodingIcon.shrink_to_fit();
	}
public:	
	static Scene* createScene();

	virtual bool init();

	void onMap();

	CREATE_FUNC(ANIPANG);

};


#endif
