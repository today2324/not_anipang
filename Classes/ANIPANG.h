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

using namespace std;
using namespace cocos2d;

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

	struct Anipang {
		Sprite* anipangIcon;
		int type;
	};

	vector<pair<int, int>> boomingIcon;
	Anipang field[ANIPANGNUM][ANIPANGNUM];
	Vec2 BeganTouch;
	bool TfuncOperate;
	int NowX, NowY;
	int bombConditions[2] = { 0, };
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
		MoveBy* IconDrop = MoveBy::create(0.3, Point(0, -ANIPANGDISTANCE));
		Blink* IconBlink = Blink::create(60, 120);
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
	void IconBoom(int first, int second);
	int matchSearch(int targetX, int targetY, int direction, Vec2 decide);
	void resetSearch(int Ytrigger);
	bool isX = true;
	void fallIconDel(int minValue);
public:	
	static Scene* createScene();

	virtual bool init();

	void onMap();

	CREATE_FUNC(ANIPANG);

};


#endif
