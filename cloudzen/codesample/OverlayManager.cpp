#include "stdafx.h"

#include "OverlayManager.h"
#include "EntertainmentOverlay.h"
using namespace cocos2d;
using namespace GameCloud;
using namespace std;

namespace
{
	struct OverlayFileType
	{
		OverlayManager::OverlayType											OverlayType;
		string																CsbFileName;
	};
}

struct OverlayManagerImp
{
	cocos2d::Node*															OverlayManagerRoot = nullptr;

	map <OverlayManager::OverlayType, shared_ptr <OverlayBase> >			Overlays;
	

	OverlayManagerImp (cocos2d::Scene & rootScene)
	{
		LogFunction;

		OverlayManagerRoot = cocos2d::Node::create ();

		GameCloudAssert (nullptr != OverlayManagerRoot);

		OverlayManagerRoot->setName ("OverlayManagerRoot");

		rootScene.addChild (OverlayManagerRoot);

		

		OverlayFileType overlayMaps [] =
		{
			{ OverlayManager::OverlayType::Entertainment,	"ui/entertainment/entertainmentcenter.csb"}
		};

		//TO FIX if else 
		// C++ Reflection??
		for (auto iter : overlayMaps)
		{
			if (OverlayManager::OverlayType::Entertainment == iter.OverlayType)
			{
				Node* nodetree = CSLoader::createNode (overlayMaps->CsbFileName);

				//shared_ptr <TestOverlay> testoverlay (new TestOverlay (OverlayManagerRoot, nodetree));

				shared_ptr <EntertainmentOverlay> entertainmentOverlay (new EntertainmentOverlay (nodetree));

				OverlayManagerRoot->addChild (nodetree);

				Overlays [overlayMaps->OverlayType] = entertainmentOverlay;
			}
		}
		
	}

	~OverlayManagerImp ()
	{
		LogFunction;
		RemoveAllChildrenRecursive (OverlayManagerRoot);
		if(OverlayManagerRoot)
			OverlayManagerRoot->removeFromParent ();
		OverlayManagerRoot = nullptr;
	}

	void																	ChangeTheme (const char * themepList)
	{
		OverlayManagerRoot->printLeaks ();
		DebugPrintScene ();

		// 		// *** cannot use removeSpriteFramesFromFile, because different plist have same data.
		// 		// This function just check data inside and remove all of it. (doesn't care plist name.)
		// 		// But, New plist name still remain in cache, so cannot add sprite frames again.  it cause crash.

		SpriteFrameCache::getInstance ()->removeSpriteFramesFromFile (themepList);
		SpriteFrameCache::getInstance ()->addSpriteFramesWithFile (themepList);
		ChangeThemeRecursive (OverlayManagerRoot);
		OverlayManagerRoot->printLeaks ();
	}

	void																	ChangeThemeRecursive (cocos2d::Node* node)
	{
		for (auto child : node->getChildren ())
		{
			ChangeThemeRecursive (child);

// 			if (Sprite * sprite = dynamic_cast<Sprite*> (child))
// 			{
//  				sprite->setSpriteFrame (ThemeSpriteMap [sprite->getName ()]);
// 				// no need already released in setSpriteFrame
// 				//oldSprite->release ();
// 			}
			if (ui::ImageView* testimageview = dynamic_cast<cocos2d::ui::ImageView*> (child))
			{
				testimageview->loadTexture (testimageview->getRenderFile ().file, ui::Widget::TextureResType::PLIST);
			}
			else if (ui::Button* testbutton = dynamic_cast<cocos2d::ui::Button*> (child))
			{
				string normalbtnname   = testbutton->getNormalFile ().file;
				string disabledbtnname = testbutton->getDisabledFile ().file;
				string pressedbtnname  = testbutton->getPressedFile ().file;
				testbutton->loadTextures (normalbtnname, disabledbtnname, pressedbtnname, ui::Widget::TextureResType::PLIST);
			}
			else if (ui::Slider* testslider = dynamic_cast<ui::Slider*> (child))
			{
				testslider->loadBarTexture         (testslider->getBackFile ().file, ui::Widget::TextureResType::PLIST);
				testslider->loadProgressBarTexture (testslider->getProgressBarFile ().file, ui::Widget::TextureResType::PLIST);
				testslider->loadSlidBallTextures   (testslider->getBallNormalFile ().file, testslider->getBallPressedFile ().file, testslider->getBackFile ().file, ui::Widget::TextureResType::PLIST);
			}
			else if (ui::CheckBox* testcheckbox = dynamic_cast<cocos2d::ui::CheckBox*> (child))
			{
				testcheckbox->loadTextures (testcheckbox->getBackNormalFile ().file, testcheckbox->getBackPressedFile ().file, testcheckbox->getCrossNormalFile ().file, testcheckbox->getBackDisabledFile ().file, testcheckbox->getCrossDisabledFile ().file, ui::Widget::TextureResType::PLIST);
			}
			else if (ui::ScrollView* testscrollview = dynamic_cast<cocos2d::ui::ScrollView*> (child))
			{
				testscrollview->setBackGroundImage (testscrollview->getRenderFile ().file, ui::Widget::TextureResType::PLIST);
			}
		}
	}

	void																	ShowOverlay (OverlayManager::OverlayType type, bool isShow)
	{
		auto it = Overlays.find (type);

		GameCloudAssert (Overlays.end () != it);

		shared_ptr <OverlayBase>  overlay = it->second;

		overlay->ShowOverlay (isShow);
	}
};

OverlayManager::OverlayManager (cocos2d::Scene & rootScene)
 : imp_ (new OverlayManagerImp (rootScene))
{
	GameCloudAssert (nullptr == Global::_OverlayManager_);
	Global::_OverlayManager_ = this;
}

OverlayManager::~OverlayManager ()
{
	delete imp_;
	imp_ = 0;
}

void														OverlayManager::ChangeTheme (const char * themepList)
{
	imp_->ChangeTheme (themepList);
}

void														OverlayManager::ShowOverlay (OverlayType type, bool isShow)
{
	imp_->ShowOverlay (type, isShow);
}

