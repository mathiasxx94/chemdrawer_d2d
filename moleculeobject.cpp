#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include "moleculeobject.h"
#include "mathhelpers.h"
#include <tuple>


Molecule::Molecule(Vector2D pos, int hybridization, int atom)
{
	AddElement(pos, hybridization, atom);
	bounding_box.left = pos.x;
	bounding_box.right = pos.x;
	bounding_box.top = pos.y;
	bounding_box.bottom = pos.y;
}

void Molecule::AddElement(Vector2D pos, int hybridization, int atom)
{
	Element element;
	element.pos2D = pos;
	element.atom = atom;
	elements.push_back(element);

	AdjustBoundingBox(pos);
}

int Molecule::GetNumberOfElements()
{
	return elements.size();
}

Element* Molecule::pGetElementByIndex(int index)
{
	return &elements.at(index);
}

void Molecule::AddBond(int firstelement, int secondelement, int bondtype)
{
	Bond bond;
	bond.firstelement = firstelement;
	bond.secondelement = secondelement;
	bond.bondtype = bondtype;

	//Calculate bond angle of created bond
	Vector2D startpos = elements.at(firstelement).pos2D;
	Vector2D endpos = elements.at(secondelement).pos2D;
	float angle = atan2(-(endpos.y - startpos.y), endpos.x - startpos.x);
	bond.bondangle = mathhelp::NormalizeAngle0To2Pi(angle);// angle;

	bonds.push_back(bond);


	Bond bond2;
	bond2.firstelement = secondelement;
	bond2.secondelement = firstelement;
	bond2.bondtype = bondtype;
	bond2.bondangle = mathhelp::NormalizeAngle0To2Pi(angle + M_PI);
	

	elements[firstelement].connectedbonds.push_back(GetBondByIndex(bonds.size() - 1));
	elements[secondelement].connectedbonds.push_back(bond2);

	//Vector2D firstvector{ endpos.x - startpos.x, -(endpos.y - startpos.y) };
	//Vector2D secondvector{ startpos.x - endpos.x, -(startpos.y - endpos.y) };
	//elements[firstelement].bondvectorfromorigin.push_back(firstvector);
	//elements[secondelement].bondvectorfromorigin.push_back(secondvector);

	elements[firstelement].connectedelements.push_back(pGetElementByIndex(secondelement)); //FIX!! push_back element object instead of pointer
	elements[secondelement].connectedelements.push_back(pGetElementByIndex(firstelement));
}

void Molecule::SetBondType(int index, int bondtype)
{
	bonds[index].bondtype = bondtype;
}

Bond Molecule::GetBondByIndex(int index)
{
	return bonds.at(index);
}

Bond* Molecule::pGetBondByIndex(int index)
{
	return &bonds.at(index);
}

int Molecule::GetNumberOfBonds()
{
	return bonds.size();
}

void Molecule::AdjustBoundingBox(Vector2D pos)
{
	if (pos.x > bounding_box.right)  bounding_box.right = pos.x;
	if (pos.x < bounding_box.left)   bounding_box.left = pos.x;
	if (pos.y > bounding_box.bottom) bounding_box.bottom = pos.y;
	if (pos.y < bounding_box.top)    bounding_box.top = pos.y;

	bounding_box_padded.left = bounding_box.left - bounding_box_limit;
	bounding_box_padded.right = bounding_box.right + bounding_box_limit;
	bounding_box_padded.top = bounding_box.top - bounding_box_limit;
	bounding_box_padded.bottom = bounding_box.bottom + bounding_box_limit;
}

RECT Molecule::GetBoundingBoxPadded()
{
	return bounding_box_padded;
}

bool Molecule::IsMoleculeHovered(int x, int y)
{
	RECT temp = GetBoundingBoxPadded();

	if (x > temp.left && x < temp.right)
		if (y > temp.top && y < temp.bottom)
			return true;
	return false;
}

std::pair<bool, int> Molecule::HoveredElement(int x, int y)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (abs(elements.at(i).pos2D.x - x) < 3)
			if (abs(elements.at(i).pos2D.y - y) < 3)
				return std::pair<bool, int>(true, i);
	}
	return std::pair<bool, int>(false, 0);
}

std::pair<int, float> Molecule::ClosestElement(int x, int y)
{
	float closestdistance = 10000;
	float distancetoelement;
	int closestindex;

	for (int i = 0; i < elements.size(); i++)
	{
		distancetoelement = std::sqrtf(std::powf((x - elements[i].pos2D.x), 2) + std::pow((y - elements[i].pos2D.y), 2));

		if (distancetoelement < closestdistance)
		{
			closestdistance = distancetoelement;
			closestindex = i;
		}
	}

	return std::pair<int, float>(closestindex, closestdistance);
}

Vector2D Molecule::GetBondPosition(int index)
{
	//Bond position is defined as center of the two bond nodes
	Vector2D midpoint = (elements[bonds[index].firstelement].pos2D + elements[bonds[index].secondelement].pos2D) / 2;
	return midpoint;
}

std::pair<int, float> Molecule::ClosestBond(int x, int y)
{
	float closestdistance = 10000;
	float distancetobond;
	int closestindex;

	for (int i = 0; i < bonds.size(); i++)
	{
		Vector2D midpoint = GetBondPosition(i);

		distancetobond = std::sqrtf(std::powf((x - midpoint.x), 2) + std::pow((y - midpoint.y), 2));

		if (distancetobond < closestdistance)
		{
			closestdistance = distancetobond;
			closestindex = i;
		}
	}

	return std::pair<int, float>(closestindex, closestdistance);
}


std::tuple<bool, int, float> Molecule::ClosestPart(int x, int y)
{
	std::pair<int, float> element = ClosestElement(x, y);
	std::pair<int, float> bond = ClosestBond(x, y);

	if (element.second < bond.second)
	{
		return std::tuple<bool, int, float>(true, element.first, element.second);
	}
	return std::tuple<bool, int, float>(false, bond.first, bond.second);
}


Vector2D Molecule::GetElementPosition(int index)
{
	return elements.at(index).pos2D;
}

std::vector<Bond> Molecule::GetBondVector()
{
	return bonds;
}

void Molecule::EraseBond(int index)
{
	bonds.erase(bonds.begin() + index);
}