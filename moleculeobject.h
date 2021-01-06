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
public:
	Molecule(Vector2D, int hybridization, int atom = elements::carbon);
	~Molecule();

	void AddElement(Vector2D, int hybridization, int atom = elements::carbon);
	int GetNumberOfElements();
	Vector2D GetElementPosition(int index);
	void AddBond(int, int, int bondtype = bond::sb);
	Bond GetBondByIndex(int index);
	int GetNumberOfBonds();
	void AdjustBoundingBox(Vector2D pos);
	RECT GetBoundingBoxPadded();
	bool IsMoleculeHovered(int x, int y);
	std::pair<bool,int> HoveredElement(int x, int y); 
	
	std::pair<int, float> ClosestElement(int x, int y);						//Takes a position, e.g. mousepos and returns index and distance of element closest to this

	//std::vector<Element> elements;
	//std::vector<Bond> bonds;

private:
	std::vector<Element> elements;
	std::vector<Bond> bonds;

	RECT bounding_box;
	RECT bounding_box_padded;
	int bounding_box_limit{ 10 };
};

extern std::vector<Molecule*> molekyler;