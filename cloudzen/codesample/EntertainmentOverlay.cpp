#include "stdafx.h"

#include "EntertainmentOverlay.h"
#include "System/RoomManager/RoomManager.h"
using namespace cocos2d;
using namespace GameCloud;
using namespace std;

EntertainmentOverlay::EntertainmentOverlay (cocos2d::Node* nodetree)
	: OverlayBase (nodetree)
{
	InitializeOverlay ();
}

EntertainmentOverlay::~EntertainmentOverlay ()
{
}

void															EntertainmentOverlay::InitializeOverlay ()
{
	Node * buttonNode = FindChildNode (OverlayRoot, "buttonback");

	GameCloudAssert (buttonNode);

	cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*> (buttonNode);

	GameCloudAssert (pButton);

	pButton->addClickEventListener (std::bind (&EntertainmentOverlay::OnTestOverlayButtonClick, this, std::placeholders::_1));
}

void															EntertainmentOverlay::OnTestOverlayButtonClick (cocos2d::Ref * sender)
{
	Global::_RoomManager_->PlayZoomOut ();

	OverlayRoot->setVisible (false);
}

