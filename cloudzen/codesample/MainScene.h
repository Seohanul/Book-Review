#pragma once

struct MainSceneImp;

class MainScene
{
	friend struct MainSceneImp;
	MainSceneImp *															imp_;

	static cocos2d::Scene *													MainSceneInstance;

public:

	static cocos2d::Scene *													GetMainSceneInstance ();

	MainScene ();
	~MainScene ();
	
	MainScene (const MainScene &) = delete;
	MainScene & operator= (const MainScene &) = delete;
};
