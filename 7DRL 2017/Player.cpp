#include "Player.hpp"

Player::Player()
{
	// arrow keys and Home, End, PageUp, PageDn
	keyBinding[sf::Keyboard::Left] = Action::MoveLeft;
	keyBinding[sf::Keyboard::Right] = Action::MoveRight;
	keyBinding[sf::Keyboard::Up] = Action::MoveUp;
	keyBinding[sf::Keyboard::Down] = Action::MoveDown;
	keyBinding[sf::Keyboard::Home] = Action::MoveLeftUp;
	keyBinding[sf::Keyboard::End] = Action::MoveLeftDown;
	keyBinding[sf::Keyboard::PageUp] = Action::MoveRightUp;
	keyBinding[sf::Keyboard::PageDown] = Action::MoveRightDown;

	// numpad keys
	keyBinding[sf::Keyboard::Numpad4] = Action::MoveLeft;
	keyBinding[sf::Keyboard::Numpad6] = Action::MoveRight;
	keyBinding[sf::Keyboard::Numpad8] = Action::MoveUp;
	keyBinding[sf::Keyboard::Numpad2] = Action::MoveDown;
	keyBinding[sf::Keyboard::Numpad7] = Action::MoveLeftUp;
	keyBinding[sf::Keyboard::Numpad1] = Action::MoveLeftDown;
	keyBinding[sf::Keyboard::Numpad9] = Action::MoveRightUp;
	keyBinding[sf::Keyboard::Numpad3] = Action::MoveRightDown;

	// vi keys
	keyBinding[sf::Keyboard::H] = Action::MoveLeft;
	keyBinding[sf::Keyboard::L] = Action::MoveRight;
	keyBinding[sf::Keyboard::K] = Action::MoveUp;
	keyBinding[sf::Keyboard::J] = Action::MoveDown;
	keyBinding[sf::Keyboard::Y] = Action::MoveLeftUp;
	keyBinding[sf::Keyboard::B] = Action::MoveLeftDown;
	keyBinding[sf::Keyboard::U] = Action::MoveRightUp;
	keyBinding[sf::Keyboard::N] = Action::MoveRightDown;

	//
	keyBinding[sf::Keyboard::Space] = Action::Interact;
	keyBinding[sf::Keyboard::Return] = Action::Interact;

	keyBinding[sf::Keyboard::G] = Action::PickUp;
	keyBinding[sf::Keyboard::Comma] = Action::PickUp;

	keyBinding[sf::Keyboard::Numpad5] = Action::Wait;
	keyBinding[sf::Keyboard::Delete] = Action::Wait;
	keyBinding[sf::Keyboard::Period] = Action::Wait;

	keyBinding[sf::Keyboard::F] = Action::Fire;

	keyBinding[sf::Keyboard::Tab] = Action::CancelFire;
	keyBinding[sf::Keyboard::Escape] = Action::CancelFire;

	keyBinding[sf::Keyboard::Q] = Action::PreviousWeapon;
	keyBinding[sf::Keyboard::W] = Action::NextWeapon;
	keyBinding[sf::Keyboard::D] = Action::DropWeapon;

	keyBinding[sf::Keyboard::E] = Action::EnterOrExit;
}

Action Player::getAction(sf::Keyboard::Key key)
{
	auto found = keyBinding.find(key);

	if (found != keyBinding.end())
		return found->second;

	return Action::Unknown;
}