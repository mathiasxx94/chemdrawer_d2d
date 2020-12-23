#pragma once
//classes for various objects which can be drawn. Molecule, text box, etc...
#include <vector>

struct Element
{
	float posX2D;
	float posY2D;
	float posX3D;
	float posY3D;
	float posZ3D;
	int atom;
	int hybridization;
};

class Molecule
{
	Molecule();
	~Molecule();
	std::vector<Element> elements;
};
