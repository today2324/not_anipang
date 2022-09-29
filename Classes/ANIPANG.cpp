#include "ANIPANG.h"
#include <algorithm>
#pragma execution_character_set("UTF-8")
USING_NS_CC;
using namespace std;

Scene* ANIPANG::createScene()
{
	Scene* scene = Scene::create();
	Scene* layer = ANIPANG::create();
	scene->addChild(layer);

	return scene;
}

bool compare(const pair<int, int>& a, const pair<int, int>& b)
{
	if (a.first == b.first)
	{
		return a.second < b.second;
	}
	return a.first < b.first;
}

void ANIPANG::allSearchDel()
{
	for (size_t i = 0; i < ANIPANGNUM; i++)
	{
		for (size_t s = 0; s < ANIPANGNUM; s++)
		{
			if (matchSearch(i, s, 0, decide))
			{
				boomingIcon.push_back(make_pair(i, s));
				sort(boomingIcon.begin(), boomingIcon.end(), compare);

				for (size_t k = 0; k < boomingIcon.size(); k++)
				{
					IconBoom(boomingIcon[k].first, boomingIcon[k].second);
				}

				i = 0, s = 0;
			}
			reset(visited);
			boomingIcon.clear();
			boomingIcon.shrink_to_fit();
			NowX += aroundX[SearchDirection];
			NowY += aroundY[SearchDirection];
			bombConditions[X] = 0;
			bombConditions[Y] = 0;
			isX = true;
		}
	}
}

void ANIPANG::onMap()
{
	decide.x = 0;
	decide.y = 0;
	Sprite* Background;
	Sprite* Wallpaper;

	Wallpaper = Sprite::create("UI/white.png");
	Wallpaper->setAnchorPoint(Point(0, 0));
	Wallpaper->setPosition(Point(0, 250));
	Wallpaper->setContentSize(Size(420, 500));
	this->addChild(Wallpaper);

	Label* exlabel = Label::createWithTTF("THIS IS NOT ANIPANG", "fonts/LeesunshinBold.ttf", 35, Size(800, 200), TextHAlignment::CENTER, TextVAlignment::CENTER);
	exlabel->setPosition(Point(210, 460));
	exlabel->setColor(Color3B::BLACK);
	this->addChild(exlabel, 100);

	Background = Sprite::create("UI/Background.png");
	Background->setPosition(Point(0, 0));
	Background->setAnchorPoint(Point(0, 0));
	Background->setContentSize(Size(420, 420));
	this->addChild(Background);

	for (int j = 0; j < ANIPANGNUM; j++)
	{
		for (int i = 0; i < ANIPANGNUM; i++)
		{
			field[i][j].type = RandomHelper::random_int(0, (int)iconName.size() - 1);

			field[i][j].anipangIcon = Sprite::create(iconName[field[i][j].type]);
			field[i][j].anipangIcon->setPosition(Point(width, height));
			field[i][j].anipangIcon->setContentSize(Size(ANIPANGSIZE, ANIPANGSIZE));
			addChild(field[i][j].anipangIcon);
			width += ANIPANGDISTANCE;
		}
		width = ANIPANGINITVALUE;
		height += ANIPANGDISTANCE;
	}
	allSearchDel();
}

void ANIPANG::setLis()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ANIPANG::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ANIPANG::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ANIPANG::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ANIPANG::onTouchBegan(Touch* touch, Event* event)
{
	if (TfuncOperate) { return false; }
	TfuncOperate = true;
	bool isTouch;
	Vec2 touchPoint = touch->getLocation();

	for (NowX = 0; NowX < ANIPANGNUM; NowX++)
	{
		Acted = false;
		for (NowY = 0; NowY < ANIPANGNUM; NowY++)
		{
			isTouch = field[NowX][NowY].anipangIcon->getBoundingBox().containsPoint(touchPoint);
			if (isTouch)
			{
				Acted = true;
				BeganTouch.x = touchPoint.x, BeganTouch.y = touchPoint.y;
				break;
			}
		}
		if (Acted)
		{
			break;
		}
	}
	return true;
}

void ANIPANG::onTouchEnded(Touch* touch, Event* event)
{
	if (!TfuncOperate) { return; }
	delIcon();
	TfuncOperate = false;

}

void ANIPANG::onTouchMoved(Touch* touch, Event* event)
{
	int PorM;
	Vec2 touchPoint = touch->getLocation();
	Vec2 swipePoint;
	Vec2 MovingDirection;

	swipePoint.x = touchPoint.x - BeganTouch.x;
	swipePoint.y = touchPoint.y - BeganTouch.y;

	if (Acted)
	{
		if (ANIPANGNUM <= NowX || NowX < 0) { return; }
		if (ANIPANGNUM <= NowY || NowY < 0) { return; }

		if (abs(swipePoint.x) > abs(swipePoint.y))
		{
			if (abs(swipePoint.x) < 30) { return; }
			if (swipePoint.x > 0)
			{
				SearchDirection = RIGHT;
				PorM = 1;
				MovingDirection.x = ANIPANGDISTANCE;
			}
			else
			{
				SearchDirection = LEFT;
				PorM = -1;
				MovingDirection.x = -ANIPANGDISTANCE;
			}
			MovingDirection.y = 0;
		}
		else
		{
			if (abs(swipePoint.y) < 30) { return; }
			MovingDirection.x = 0;
			if (swipePoint.y > 0)
			{
				SearchDirection = UP;
				PorM = 1;
				MovingDirection.y = ANIPANGDISTANCE;
			}
			else
			{
				SearchDirection = DOWN;
				PorM = -1;
				MovingDirection.y = -ANIPANGDISTANCE;
			}
		}
		moveIcon(MovingDirection, PorM);

		Acted = false;
	}
}

void ANIPANG::moveIcon(Vec2 MovingDirection, int PorM)
{
	int temp;
	int replaceX = NowX;
	int	replaceY = NowY;
	Sprite* Icontemp;
	if (MovingDirection.x) { replaceX += PorM; }
	else { replaceY += PorM; }

	if (0 > replaceX || replaceX >= ANIPANGNUM) { return; }
	if (0 > replaceY || replaceY >= ANIPANGNUM) { return; }

	MoveBy* swipeOne = MoveBy::create(0.3, Point(MovingDirection.x, MovingDirection.y));
	MoveBy* swipeTwo = MoveBy::create(0.3, Point(-MovingDirection.x, -MovingDirection.y));

	field[NowX][NowY].anipangIcon->runAction(swipeOne);
	field[replaceX][replaceY].anipangIcon->runAction(swipeTwo);

	swap(field[NowX][NowY], field[replaceX][replaceY]);
}

void ANIPANG::delIcon()
{
	decide.x = 0;
	decide.y = 0;
	vector<pair<int, int>> firstTemp;
	bool isFirstMatch, isSecondMatch;//matchSearch bool contain;

	isFirstMatch = matchSearch(NowX, NowY, 0, decide);
	log("size = %d ", boomingIcon.size());
	firstTemp = boomingIcon;
	isSecondMatch = matchSearch(NowX + aroundX[SearchDirection], NowY + aroundY[SearchDirection], 0, decide);

	if (isFirstMatch && !boomingIcon.size()) // if isSecondMatch is empty, to overflow prevent
	{
		boomingIcon = firstTemp;
	}

	if (isFirstMatch || isSecondMatch)
	{
		log("boom");
		sort(boomingIcon.begin(), boomingIcon.end(), compare);

		for (size_t k = 0; k < boomingIcon.size(); k++)
		{
			IconBoom(boomingIcon[k].first, boomingIcon[k].second);
		}

		//fallingIconDel(needDelSearch(boomingIcon));
	}
	else
	{
		//log("no boom");
	}
	boomingIcon.clear();
	boomingIcon.shrink_to_fit();
}

void ANIPANG::fallingIconDel(CoorTool need)
{

}

CoorTool ANIPANG::needDelSearch(vector<pair<int, int>> target)
{
	CoorTool needTool;

	return needTool;
}

void ANIPANG::IconBoom(int first, int second)
{
	Hide* IconHide = Hide::create();
	Show* IconShow = Show::create();
	MoveBy* IconDrop = MoveBy::create(0.3, Point(0, -ANIPANGDISTANCE));
	MoveBy* IconPullUp = MoveBy::create(0.3, Point(0, ANIPANGDISTANCE * (ANIPANGNUM - second)));
	Sequence* RaiseLowerOne = Sequence::create(IconHide, IconPullUp, IconShow, IconDrop, nullptr);
	if (boomingIcon[0].first == boomingIcon[1].first)
	{
		IconPullUp = MoveBy::create(0.3, Point(0, ANIPANGDISTANCE * (ANIPANGNUM - second + (3 - first))));
	}

	field[first][second].anipangIcon->runAction(RaiseLowerOne);
	field[first][second].type = RandomHelper::random_int(0, (int)iconName.size() - 1);
	field[first][second].anipangIcon->setTexture(iconName[field[first][second].type]);
	field[first][second].anipangIcon->setContentSize(Size(ANIPANGSIZE, ANIPANGSIZE));

	for (size_t i = second + 1; i < ANIPANGNUM; i++)
	{
		IconFall(field[first][i].anipangIcon);
	}

	for (size_t i = second + 1; i < ANIPANGNUM; i++)
	{
		if (i != ANIPANGNUM - 1)
		{
			swap(field[first][i], field[first][i + 1]);
		}
		else
		{
			swap(field[first][i], field[first][second]);
		}
	}
}

int ANIPANG::matchSearch(int targetX, int targetY, int direction, Vec2 decide)
{
	int deX = decide.x, deY = decide.y;
	int replaceX = targetX + aroundX[direction];
	int replaceY = targetY + aroundY[direction];

	//boom
	if ((bombConditions[X] >= 2 || bombConditions[Y] >= 2) && 4 <= direction)
	{
		// if Y can't boom Y value erase
		if (bombConditions[Y] < 2 && bombConditions[Y])
		{
			boomingIcon.pop_back();
		}
		resetSearch(false);
		boomingIcon.push_back(make_pair(targetX, targetY));
		return true;
	}

	//X explode trigger disappear
	if (direction > 1 && isX && !(bombConditions[X] >= 2))
	{
		for (size_t i = 0; i < bombConditions[X]; i++)
		{
			boomingIcon.pop_back();
		}
		isX = false;
		bombConditions[X] = 0;
	}

	//no boom
	if (4 <= direction || direction < 0)
	{
		resetSearch(bombConditions[Y]);
		return false;
	}

	//top bottom disconnect
	if (ANIPANGNUM <= replaceX + deX || replaceX + deX < 0)
	{
		decide.x = 0;
		return matchSearch(targetX, targetY, direction + 1, decide);
	}
	if (ANIPANGNUM <= replaceY + deY || replaceY + deY < 0)
	{
		decide.y = 0;
		return matchSearch(targetX, targetY, direction + 1, decide);
	}

	if (field[targetX][targetY].type == field[replaceX + deX][replaceY + deY].type)
	{
		if (visited[replaceX + deX][replaceY + deY])
		{
			return matchSearch(targetX, targetY, direction + 1, decide);
		}
		if (targetX != replaceX + deX || targetY != replaceY + deY)
		{
			boomingIcon.push_back(make_pair(replaceX + deX, replaceY + deY));
		}

		visited[replaceX + deX][replaceY + deY] = true;

		switch (direction)
		{
		case RIGHT:
			decide.x = deX + 1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[X]++;
			break;
		case LEFT:
			decide.x = deX - 1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[X]++;
			break;
		case UP:
			decide.y = deY + 1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[Y]++;
			break;
		case DOWN:
			decide.y = deY - 1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[Y]++;
			break;
		default:
			break;
		}

		return matchSearch(targetX, targetY, direction, decide);
	}
	else
	{
		decide.x = 0;
		decide.y = 0;

		return matchSearch(targetX, targetY, direction + 1, decide);
	}
}

void ANIPANG::resetSearch(int Ytrigger)
{
	bombConditions[X] = 0;
	bombConditions[Y] = 0;
	reset(visited);
	isX = true;
	if (Ytrigger)
	{
		boomingIcon.pop_back();
	}

}

void ANIPANG::onExit()
{
	_eventDispatcher->removeAllEventListeners();
}

bool ANIPANG::init()
{
	if (!Scene::init())
	{
		return false;
	}

	onMap();
	setLis();
	//onExit();
	return true;
}