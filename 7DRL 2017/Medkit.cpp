#include "Medkit.hpp"
#include "Actor.hpp"

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Medkit::Medkit()
	: Item(L'!', L'!', Color::Yellow)
{
}

ItemType Medkit::getItemType() const
{
	return ItemType::Medkit;
}

std::wstring Medkit::getName(Article article) const
{
	return attachArticle(Strings::get("Medkit"), article);
}

bool Medkit::apply(Actor& actor)
{
	world->getLog().print(Strings::get("UseMedkit"), Color::Yellow, { getName(Article::A) });

	if (actor.getHp() >= actor.getMaxHp() * 9 / 10)
	{
		actor.changeMaxHp(1);
		actor.restoreHp(actor.getMaxHp());
	}

	else
		actor.restoreHp(actor.getMaxHp() / 3);

	return true;
}