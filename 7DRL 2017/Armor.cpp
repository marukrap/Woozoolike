#include "Armor.hpp"
#include "Actor.hpp"
#include "Utility.hpp" // rng

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Armor::Armor(Type type)
	: Item(L']', L']', Color::Yellow)
	, type(type)
	, defense(0)
	, maxDefense(0)
{
	// TODO: make armor data table
	switch (type)
	{
	case Helmet:
		glyph = L'[';
		tileNumber = L'[';
		defense = randomInt(1, 3);
		maxDefense = 3;
		break;

	case Shield:
		glyph = L'{';
		tileNumber = L'{';
		defense = randomInt(2, 5);
		maxDefense = 5;
		break;

	case Body:
		glyph = L']';
		tileNumber = L']';
		defense = randomInt(3, 7);
		maxDefense = 7;
		break;
	}
}

ItemType Armor::getItemType() const
{
	return ItemType::Armor;
}

Armor::Type Armor::getArmorType() const
{
	return type;
}

std::wstring Armor::getName(Article article) const
{
	std::wstring name;

	switch (type)
	{
	case Shield:	name = Strings::get("Shield"); break;
	case Helmet:	name = Strings::get("Helmet"); break;
	case Body:		name = Strings::get("BodyArmor"); break;
	}

	// TODO: 한글 처리 기호 [, ] 와 섞여 오류 발생
	// name += L" [" + std::to_wstring(defense) + L"]";

	if (type == Body && article == Article::A)
		article = Article::None;

	return attachArticle(name, article);
}

void Armor::changeDefense(int points)
{
	defense += points;

	if (defense > maxDefense)
		defense = maxDefense;
	else if (defense < 0)
		defense = 0;
}

int Armor::getDefense() const
{
	return defense;
}

int Armor::getMaxDefense() const
{
	return maxDefense;
}

bool Armor::apply(Actor& actor)
{
	actor.equipment.setArmor(type, this);
	
	Weapon* weapon = actor.equipment.getCurrentWeapon();

	if (getArmorType() == Shield && weapon && weapon->getWeaponType() > Weapon::Pistol)
		world->getLog().print(Strings::get("PickUpArmor"), Color::Yellow, { getName(Article::A) });
	else
		world->getLog().print(Strings::get("WearArmor"), Color::Yellow, { getName(Article::A) });

	return false;
}