#include "moleculeobject.h"


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

void Molecule::AdjustBoundingBox(Vector2D pos)
{
	pos.x > bounding_box.right ? bounding_box.right = pos.x : bounding_box.left = pos.x;
	pos.y > bounding_box.bottom ? bounding_box.bottom = pos.y : bounding_box.top = pos.y;

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