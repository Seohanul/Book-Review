#include "stdafx.h"

#include "RoomManager.h"


#include "animation/AnimationManager.h"
#include "animation/AnimateClip.h"

//test for overlay
#include "Test/TestUI/TestOverlay.h"

using namespace cocos2d;
using namespace GameCloud;
using namespace std;

namespace
{
	enum ButtonType
	{
		  MenuEntertainment
		, MenuPersonal
		, MenuSocial
		, MenuLiveCast
		, MenuSystemNotification
	};

	struct RoomButtonCallbackType
	{
		ButtonType		ButtonID;
		std::string		ButtonName;
	};
}

struct RoomManagerImp
{
	creator::AnimationManager *												AnimationManager_ = nullptr;
	bool																	IsZoomed = false;

	vector< cocos2d::ui::Button*>											MenuButtons;

// Should we create a Scene * instead of Node
	cocos2d::Node*															RoomManagerRoot = nullptr;

	RoomManagerImp ( cocos2d::Scene & rootScene)
	{
		LogFunction;

		RoomManagerRoot = cocos2d::Node::create ();

		GameCloudAssert (nullptr != RoomManagerRoot);

		RoomManagerRoot->setName ("RoomManagerRoot");

		rootScene.addChild (RoomManagerRoot);
	}

	~RoomManagerImp ()
	{
		LogFunction;

		UnloadScene ();

// TOFIX 
		if (AnimationManager_)
			AnimationManager_->release ();
		AnimationManager_ = nullptr;

		if (RoomManagerRoot)
		{
			// will crash if we do not remove children in Android (not on windowS)
			//RemoveAllChildrenRecursive (RoomManagerRoot);
			RoomManagerRoot->removeFromParent ();
			// removeFromParent will already release
			//RoomManagerRoot->release ();
		}
		RoomManagerRoot = nullptr;

	}

	void																	EnableAllMenuButtons (bool enable)
	{
		for (int i = 0; i < MenuButtons.size (); ++i)
		{
			MenuButtons [i]->setTouchEnabled (enable);
			MenuButtons [i]->setEnabled (enable);
		}
	}
// 	void																	EnterRoom (const char * fileName)
// 	{
// 		SpriteFrameCache::getInstance ()->removeUnusedSpriteFrames ();
// 		Director::getInstance ()->purgeCachedData ();
// 
// 		creator::CreatorReader *  CreatorReader = creator::CreatorReader::createWithFilename (fileName);
// 		AnimationManager_ = CreatorReader->getAnimationManager ();
// 
// 		GameCloudAssert (CreatorReader);
// 		CreatorReader->setup ();
// 
// 		Scene* scene = CreatorReader->getSceneGraph ();
// 		
// 		scene->setName ("RoomNode");
// 		RoomManagerRoot->addChild (scene);
// 		//	DebugPrintScene ();
// 
// 		SetupButtons ();
// 
// 		SpriteFrameCache::getInstance ()->removeUnusedSpriteFrames ();
// 		Director::getInstance ()->purgeCachedData ();
// 	}
	void																	LoadScene (const char * fileName)
	{
		// for animation
// 		Node * roomRootNode = FindChildNode (RoomManagerRoot, "roomroot");
// 
// 		if (roomRootNode)
// 		{
// 			AnimationManager_->playAnimationClip (roomRootNode, "roomreverse");
// 			auto testexit = AnimationManager_->getAnimateClip (roomRootNode, "roomreverse");
// 			testexit->setCallbackForEndevent ([=]
// 			{
// 				//Node * roomRootNode = FindChildNode (RoomManagerRoot, "roomroot");
// 				AnimationManager_->removeAnimateClip (roomRootNode, "roomreverse");
// 				// do more
// 				RemoveAllChildrenRecursive (RoomManagerRoot);
// 
// 				EnterRoom (fileName);
// 			});
// 		}
// 		else
// 			EnterRoom (fileName);

		SpriteFrameCache::getInstance ()->removeUnusedSpriteFrames ();
		Director::getInstance ()->purgeCachedData ();

		creator::CreatorReader *  CreatorReader = creator::CreatorReader::createWithFilename (fileName);

		//FIXED 
		if (AnimationManager_)
			AnimationManager_ = nullptr;
		AnimationManager_ = CreatorReader->getAnimationManager ();

		GameCloudAssert (CreatorReader);
		CreatorReader->setup ();

		Scene* scene = CreatorReader->getSceneGraph ();
		
		scene->setName ("RoomNode");
		RoomManagerRoot->addChild (scene);
		//	DebugPrintScene ();

		SetupButtons ();

		SpriteFrameCache::getInstance ()->removeUnusedSpriteFrames ();
		Director::getInstance ()->purgeCachedData ();

		DebugPrintScene ();
	}

	void																	PlayZoomIn (ui::Button* button)
	{
		Node* roomnode = FindChildNode (RoomManagerRoot, "roomroot");
		GameCloudAssert (nullptr != roomnode);

		float dur = 0.4f;

// TOFIX - KK , disney room have anchor point Zero, but living room have 0.5,0.5 
		cocos2d::Point startPos = button->convertToWorldSpace (Point::ZERO);
		float zoomInScale = 2;
		//Zoom In Animation
		cocos2d::ScaleTo* scale = cocos2d::ScaleTo::create (dur, zoomInScale);
		cocos2d::MoveTo* move = cocos2d::MoveTo::create (dur, -startPos);
		cocos2d::Spawn* spawn = cocos2d::Spawn::create (move, scale, nullptr);
		roomnode->runAction (cocos2d::Sequence::create (spawn, nullptr));
	}

	void																	RoomMenuSelectedCallback (cocos2d::Ref* pSender)
	{
		cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*>(pSender);
		GameCloudAssert (nullptr != pButton);

		PlayZoomIn (pButton);

		ButtonType buttonType = (ButtonType)pButton->getActionTag ();

		switch (buttonType)
		{
		case ButtonType::MenuEntertainment:
			Global::_OverlayManager_->ShowOverlay (OverlayManager::OverlayType::Entertainment, true);
			break;
			//TODO
		default:
			break;
		}

		EnableAllMenuButtons (false);
	}

	void																	SetupButtons ()
	{
		RoomButtonCallbackType buttonsName [] =
		{
			{ ButtonType::MenuEntertainment,		"btnentertainmentcenterzoom" },
			{ ButtonType::MenuPersonal,				"btnusercenterzoom" },
			{ ButtonType::MenuSocial,				"btnfriendszoom" },
			{ ButtonType::MenuSystemNotification,	"btnsystemnotificationzoom" }
		};
		MenuButtons.clear ();
		for (int i = 0; i < sizeof (buttonsName) / sizeof (buttonsName [0]); ++i)
		{
			Node * buttonNode = GameCloud::FindChildNode (RoomManagerRoot, buttonsName [i].ButtonName);

			GameCloudAssertMsg (nullptr != buttonNode, "Child node not found:" << buttonsName [i].ButtonName);

			cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*> (buttonNode);

			GameCloudAssertMsg (nullptr != pButton, "Child button not found:" << buttonsName [i].ButtonName);

			pButton->addClickEventListener (std::bind (&RoomManagerImp::RoomMenuSelectedCallback, this, std::placeholders::_1));

			pButton->setActionTag (buttonsName [i].ButtonID);

			MenuButtons.push_back (pButton);
		}
	}

	void																	UnloadScene ()
	{

		// It's for reverse room animation 
// 		if (AnimationManager_)
// 		{
// 			Node * roomRootNode = FindChildNode (RoomManagerRoot, "roomroot");
// 
// 			if (roomRootNode)
// 			{
// 				AnimationManager_->playAnimationClip (roomRootNode, "roomreverse");
// 				auto test = AnimationManager_->getAnimateClip (roomRootNode, "roomreverse");
// 				test->setCallbackForEndevent ([&]
// 				{
// 					AnimationManager_->removeAnimateClip (roomRootNode, "roomreverse");
// 
// 					// do more
// 					RemoveAllChildrenRecursive (RoomManagerRoot);
// 				});
// 			}
// 		}

// No need to release, it is part of room scene		

		// TOFIX
/* 		AnimationManager_ = nullptr;*/
// 		// function for delete all children (including grandchildren)
		RemoveAllChildrenRecursive (RoomManagerRoot);
		// 		// This does not cleanup the vector children properly
		// 		//RoomManagerRoot->removeAllChildren ();
		SpriteFrameCache::getInstance ()->removeUnusedSpriteFrames ();
// This is called in dtor, Director::getInstance will create a new instance
//		Director::getInstance ()->purgeCachedData ();
	}
};

RoomManager::RoomManager(cocos2d::Scene & rootScene)
	:imp_ (new RoomManagerImp (rootScene))
{
	GameCloudAssert (nullptr == Global::_RoomManager_);
	Global::_RoomManager_ = this;
}

RoomManager::~RoomManager ()
{
	delete imp_;
	imp_ = nullptr;
}

void RoomManager::LoadScene (const char * filename)
{
	imp_->LoadScene (filename);
}
void															RoomManager::PlayZoomOut ()
{
	// This function for using zoom Out in TestOverlay callback.
	Node* roomnode = FindChildNode (imp_->RoomManagerRoot, "roomroot");

	GameCloudAssert (roomnode);

	float dur = 0.4f;
	float zoomoutscale = 1;
	cocos2d::Point endPos = { 0,0 };
	cocos2d::ScaleTo* outScale = cocos2d::ScaleTo::create (dur, zoomoutscale);
	cocos2d::MoveTo* outMove = cocos2d::MoveTo::create (dur, endPos);
	cocos2d::Spawn* outSpawn = cocos2d::Spawn::create (outScale, outMove, nullptr);
	auto enablebtnfunc = CallFunc::create ([&] () {imp_->EnableAllMenuButtons (true); });
	roomnode->runAction (cocos2d::Sequence::create (outSpawn, enablebtnfunc, nullptr));
}

void RoomManager::UnLoadScene ()
{
	imp_->UnloadScene ();
}
