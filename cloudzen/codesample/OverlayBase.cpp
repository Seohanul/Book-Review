#include "stdafx.h"

#include "OverlayBase.h"

using namespace cocos2d;
using namespace GameCloud;
using namespace std;

void OverlayBase::ShowOverlay (bool visible)
{
	OverlayRoot->setVisible (visible);
}
OverlayBase::OverlayBase ()
{}


OverlayBase::OverlayBase ( cocos2d::Node* nodetree)
{
	OverlayRoot = nodetree;
	OverlayRoot->setVisible (false);
	InitializeOverlay ();
}

OverlayBase::~OverlayBase ()
{

}

