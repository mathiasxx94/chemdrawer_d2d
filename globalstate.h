#pragma once

struct GlobalState
{
	bool objectishovered{ false };
	bool atomishovered{ false };
	int hoveredobject;					//index into molekyler vector
	int hoveredatom;					//index into elements vector
	int curreditetobject;
	int curreditedatom;					//used to get start index when new bond is created
	bool editingisactive{ false };
	bool newobjectcreated{ false };

	bool needsrerender{ false };
};

extern GlobalState cdglobalstate;