#pragma once
//classes for various objects which can be drawn. Molecule, text box, etc...

#include <Windows.h>
#include <vector>
#include "Vector.h"
#include "chemproperties.h"



struct Bond; //Forward declaration of struct for use in bond
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
	std::vector<Bond*> connectedbonds;
};

struct Bond
{
	int firstelement;
	int secondelement;
	int bondtype;
	float bondangle; // Counterclockwise 0-180 degrees, and clockwise 0-(-180) degrees
};

class Molecule
{
public:
	Molecule(Vector2D, int hybridization, int atom = elements::carbon);
	~Molecule();

	// Element methods
	void AddElement(Vector2D, int hybridization, int atom = elements::carbon);
	int GetNumberOfElements();
	Element* pGetElementByIndex(int index);
	Vector2D GetElementPosition(int index);

	//Bond methods
	void AddBond(int, int, int bondtype = bond::sb);
	void SetBondType(int index, int bondtype);
	Bond GetBondByIndex(int index);
	Bond* pGetBondByIndex(int index);
	Vector2D GetBondPosition(int index);
	int GetNumberOfBonds();
	std::vector<Bond> GetBondVector();
	void EraseBond(int index);

	//Bounding box
	void AdjustBoundingBox(Vector2D pos);
	RECT GetBoundingBoxPadded();

	//Objecthovering
	bool IsMoleculeHovered(int x, int y);
	std::pair<bool,int> HoveredElement(int x, int y); 
	
	std::pair<int, float> ClosestElement(int x, int y);						//Takes a position, e.g. mousepos and returns index and distance of element closest to this
	std::pair<int, float> ClosestBond(int x, int y);
	std::tuple<bool, int, float> ClosestPart(int x, int y);					//Bool: 0 is element 1 is bond

private:
	std::vector<Element> elements;
	std::vector<Bond> bonds;

	RECT bounding_box;
	RECT bounding_box_padded;
	int bounding_box_limit{ 10 };
};

extern std::vector<Molecule*> molekyler;