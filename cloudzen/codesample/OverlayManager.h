#pragma once

struct OverlayManagerImp;

class OverlayManager
{
	OverlayManagerImp *														imp_;

public:

	enum OverlayType
	{
		Entertainment,
		Friends,
		User,
		System,
		Test,

		End
	};

	OverlayManager (cocos2d::Scene & rootScene);
	~OverlayManager ();
	
	OverlayManager (const OverlayManager &) = delete;
	OverlayManager & operator= (const OverlayManager &) = delete;

	void																	ChangeTheme (const char * themepList);

	void																	ShowOverlay (OverlayType type, bool isShow);

};
