#pragma once

struct GlobalState
{
	bool objectishovered{ false };
	bool atomishovered{ false };
	int hoveredobject; //index into molekyler vector
	int hoveredatom; //index into elements vector

	bool needsrerender{ 0 };
};

extern GlobalState cdglobalstate;