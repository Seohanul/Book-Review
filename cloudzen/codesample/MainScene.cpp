#include "stdafx.h"

#include "MainScene.h"
#include "Scene/SplashScene.h"

#include "System/RoomManager/RoomManager.h"


using namespace cocos2d;
using namespace GameCloud;
using namespace std;

namespace
{
	// temporary thing for changing scene with buttons
	map <int, string>														TestScenesMap = {
		{0, "creator/room/disneyroom/room.ccreator" },
		{1, "creator/room/modernlivingroom/room.ccreator" }
		// other scenes.
	};
//x
	//temporary thing for changing theme with one button
	map <int, string>														TestTheme = {
		{0, "theme/modern/button/modern.plist" },
		{1, "theme/water/button/water.plist" }
	};
}

cocos2d::Scene *															MainScene::MainSceneInstance = nullptr;

cocos2d::Scene *															MainScene::GetMainSceneInstance ()
{
	GameCloudAssert (MainSceneInstance);
	return MainSceneInstance;
}

struct MainSceneImp : public Scene
{
	MainScene &																self_;

	unique_ptr <RoomManager>												roomManager_;

	unique_ptr <OverlayManager>												overlayManager_;

	MainSceneImp (MainScene & self)
		: self_ (self)
	{
		LogFunction;

		GameCloudAssert (nullptr == MainScene::MainSceneInstance);
		MainScene::MainSceneInstance = this;

		Director * director = Director::getInstance ();
		if (nullptr == director->getRunningScene ())
		{
			director->runWithScene (this);
			// force getRunningScene to be valid
			director->drawScene ();
		}
		Scene::init ();

		this->setName ("MainScene");
		
		/////////////////

		auto visibleSize = Director::getInstance ()->getVisibleSize ();
		Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

 		roomManager_.reset (new RoomManager (*this));
		overlayManager_.reset (new OverlayManager (*this));
 
		{
			for (int i = 0; i < TestScenesMap.size (); ++i)
			{
				ui::Button* loadbtn = ui::Button::create ("test/CloseNormal.png");
				loadbtn->setPosition (Vec2 (300.f + i * 50, 200.f));
				loadbtn->addClickEventListener ([&] (cocos2d::Ref* pSender) 
				{
					cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*>(pSender);

					roomManager_->UnLoadScene ();

					LogFunction;
					printLeaks ();
					LogMessage ("--Before loading--");

					int id = pButton->getActionTag ();

					roomManager_->LoadScene (TestScenesMap [id].c_str ());

					LogMessage ("--Finished loading--");
// 					printLeaks ();
				});
				loadbtn->setTitleText (make_cstr (i));
				loadbtn->setActionTag (i);
				this->addChild (loadbtn, 1);
			}
		}

		{
			ui::Button* unloadbtn = ui::Button::create ("test/CloseNormal.png");
			unloadbtn->setPosition (Vec2 (400.f, 100.f));
			unloadbtn->addClickEventListener ([&] (cocos2d::Ref* pSender)
			{
				LogMessage ("--Before unload--");
				//printLeaks ();

				roomManager_->UnLoadScene ();

				LogMessage ("--Print Scene --");
				DebugPrintScene ();
				LogMessage ("--Finished unload--");
				//printLeaks ();
			});
			unloadbtn->setTitleText ("UnLoad");
			//	btn->setActionTag (i);
			this->addChild (unloadbtn, 1);
		}

		{
			for (int i = 0; i < TestTheme.size (); ++i)
			{
				ui::Button* changeThemebtn = ui::Button::create ("test/CloseNormal.png");
				changeThemebtn->setPosition (Vec2 (400.f + i * 50, 400.f));
				changeThemebtn->addClickEventListener ([&] (cocos2d::Ref* pSender)
				{
					cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*>(pSender);
					int id = pButton->getActionTag ();
					//temporary thing for changing theme with one button
					Global::_OverlayManager_->ChangeTheme (TestTheme [id].c_str ());
				});
				changeThemebtn->setTitleText ("Theme");
				changeThemebtn->setActionTag (i);
				this->addChild (changeThemebtn, 1);
			}
		}

		{
			ui::Button* btn = ui::Button::create ("test/CloseNormal.png");
			//btn->setPosition (Vec2 (200.f, 200.f));
			btn->setPosition (Vec2 (0 + btn->getContentSize ().width / 2, visibleSize.height - btn->getContentSize ().height / 2));
			btn->addClickEventListener ([] (Ref* sender)
			{
				LogDebug ("-- Quit button pressed");

				Director::getInstance ()->end ();
			});
			this->addChild (btn);
		}
	}
	
	virtual ~MainSceneImp ()
	{
		LogFunction;
	}

	void																	OnButtonPressed (cocos2d::Ref* pSender)
	{
		LogFunction;

		ui::Widget * widget = dynamic_cast <ui::Widget*> (pSender);

		GameCloudAssert (nullptr != widget);

		int actionTag = widget->getActionTag ();

		switch (actionTag)
		{
		case 0:

			//LogFunction;

			Director::getInstance ()->replaceScene (TransitionFade::create (0.5f, SplashScene::GetSplashSceneInstance (), Color3B (0, 255, 255)));

			break;

		default:	GameCloudAssertMsg (0, "Unknown id:" << actionTag); break;
		}
	}

	void																	onEnter () override
	{
		LogFunction;
		Node::onEnter ();
	}

	void																	onExit () override
	{
		LogFunction;
		Node::onExit ();
		//printLeaks ();
	}
};

MainScene::MainScene ()
 : imp_ (new MainSceneImp (*this))
{
}

MainScene::~MainScene ()
{
	delete imp_;
	imp_ = nullptr;
}
