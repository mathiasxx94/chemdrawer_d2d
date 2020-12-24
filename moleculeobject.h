#pragma once
//classes for various objects which can be drawn. Molecule, text box, etc...

#include <Windows.h>
#include <vector>
#include "Vector.h"
#include "chemproperties.h"




struct Element
{
	Vector2D pos2D;
	Vector3D pos3D;
	float posX2D;
	float posY2D;
	float posX3D;
	float posY3D;
	float posZ3D;
	int atom;
	int hybridization;
};

struct Bond
{
	int firstelement;
	int secondelement;
	int bondtype;
};

class Molecule
{
	Molecule(Vector2D, int hybridization, int atom = elements::carbon);
	~Molecule();

	void AddElement(Vector2D, int hybridization, int atom);
	void AdjustBoundingBox(Vector2D pos);
	RECT GetBoundingBoxPadded();
	bool IsMoleculeHovered(int x, int y);
	std::pair<bool,int> HoveredElement(int x, int y); 

	std::vector<Element> elements;
private:
	RECT bounding_box;
	RECT bounding_box_padded;
	int bounding_box_limit{ 5 };
};

extern std::vector<Molecule*> molekyler;