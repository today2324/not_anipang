#include "ANIPANG.h"
#pragma execution_character_set("UTF-8")
USING_NS_CC;

Scene* ANIPANG::createScene()
{
	Scene* scene = Scene::create();
	Scene* layer = ANIPANG::create();
	scene->addChild(layer);

	return scene;
}

void ANIPANG::onMap()
{
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

	temp = field[NowX][NowY].type;
	field[NowX][NowY].type = field[replaceX][replaceY].type;
	field[replaceX][replaceY].type = temp;

	Icontemp = field[NowX][NowY].anipangIcon;
	field[NowX][NowY].anipangIcon = field[replaceX][replaceY].anipangIcon;
	field[replaceX][replaceY].anipangIcon = Icontemp;
}

void ANIPANG::delIcon()
{
	Vec2 decide;
	decide.x = 0;
	decide.y = 0;

	for (size_t i = 0; i < 2; i++)
	{
		if (matchSearch(NowX, NowY, 0, decide))
		{
			boomingIcon.push_back(make_pair(NowX, NowY));
			log("boom");
		
			for (size_t j = 0; j < boomingIcon.size(); j++)
			{
				log("[boom] Icon direction = %d, %d", boomingIcon[j].first, boomingIcon[j].second);
			}

			for (size_t k = 0; k < boomingIcon.size(); k++)
			{
				log("size = %d", boomingIcon.size());
				IconBoom(boomingIcon[k].first, boomingIcon[k].second);
			}
			boomingIcon.clear();
			boomingIcon.shrink_to_fit();
		}
		else
		{
			log("no boom");
		}
		reset(visited);
		NowX += aroundX[SearchDirection];
		NowY += aroundY[SearchDirection];
		bombConditions[X] = 0;
		bombConditions[Y] = 0;
		isX = true;
	}
}

void ANIPANG::IconBoom(int first, int second)
{
	Hide* IconHide = Hide::create();
	Blink* IconBlink = Blink::create(60, 120);
	Show* IconShow = Show::create();
	Sequence* IconVisible = Sequence::create(IconBlink, IconShow);
	field[first][second].anipangIcon->runAction(/*IconVisible*/IconBlink);
}

int ANIPANG::matchSearch(int targetX, int targetY, int direction, Vec2 decide)
{
	int deX=decide.x, deY=decide.y;
	int replaceX = targetX + aroundX[direction];
	int replaceY = targetY + aroundY[direction];
	
	if (4 <= direction || direction < 0)
	{
		return false;
	}

	//X explode trigger disappear
	if (direction > 1 && isX)
	{
		isX = false;
		boomingIcon.clear();
		boomingIcon.shrink_to_fit();
		bombConditions[X] = 0;
	}

	if (bombConditions[X] >= 2 || bombConditions[Y] >= 2)
	{
		return true;
	}

	if (!visited[targetX][targetY])
	{
		visited[targetX][targetY] == true;
	}

	//top bottom disconnect
	if (ANIPANGNUM <= replaceX+deX || replaceX+deX < 0)
	{
		log("X fff [direction = %d]", direction);
		deX = 0;
		return matchSearch(targetX, targetY, direction + 1, decide);
	}
	if (ANIPANGNUM <= replaceY + deY || replaceY + deY < 0)
	{
		log("Y fff [direction = %d]", direction);
		deY = 0;
		return matchSearch(targetX, targetY, direction + 1, decide);
	}

	if (field[targetX][targetY].type == field[replaceX + deX][replaceY + deY].type)
	{
		if (visited[replaceX + deX][replaceY + deY])
		{
			log("visited Icon");
			return matchSearch(targetX, targetY, direction + 1, decide);
		}
		if (targetX != replaceX + deX || targetY != replaceY + deY)
		{
			boomingIcon.push_back(make_pair(replaceX + deX, replaceY + deY));
		}

		log("[input vector value] %d %d", replaceX + deX, replaceY + deY);
		visited[replaceX + deX][replaceY + deY] = true;

		switch (direction)
		{
		case RIGHT:
			decide.x = deX+1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[X]++;
			break;
		case LEFT:
			decide.x = deX-1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[X]++;
			break;
		case UP:
			decide.y = deY+1;
			if (targetX == replaceX + deX && targetY == replaceY + deY)
			{
				return matchSearch(targetX, targetY, direction, decide);
			}
			bombConditions[Y]++;
			break;
		case DOWN:
			decide.y = deY-1;
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

	Acted = true;
	return 1;
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
