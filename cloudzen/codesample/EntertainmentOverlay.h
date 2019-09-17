#pragma once

struct EntertainmentOverlayImp;

class EntertainmentOverlay : public OverlayBase
{
	EntertainmentOverlayImp *												imp_;

	void																	InitializeOverlay () override;

	void																	OnTestOverlayButtonClick (cocos2d::Ref * sender);
public:

	EntertainmentOverlay (cocos2d::Node* nodetree);
	~EntertainmentOverlay ();

	EntertainmentOverlay (const EntertainmentOverlay &) = delete;
	EntertainmentOverlay & operator= (const EntertainmentOverlay &) = delete;
};
