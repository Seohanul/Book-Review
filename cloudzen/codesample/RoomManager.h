#pragma once

struct RoomManagerImp;

class RoomManager
{
	RoomManagerImp *														imp_;
	//void playZoomAnimation (cocos2d::ui::Button* pButton);
public:

	RoomManager (cocos2d::Scene & rootScene);
	~RoomManager ();
	
	RoomManager (const RoomManager &) = delete;
	RoomManager & operator= (const RoomManager &) = delete;

	void																	LoadScene (const char * filename);
	void																	UnLoadScene ();
	void																	PlayZoomOut ();
};
