#pragma once

class OverlayBase 
{
protected:
	cocos2d::Node*															OverlayRoot = nullptr;

	virtual	void															InitializeOverlay () {}
public:
	OverlayBase ();
	OverlayBase ( cocos2d::Node* nodetree);
	virtual ~OverlayBase ();

	void																	ShowOverlay (bool visible);
};
