#include "steam_user.hpp"

steam_user::steam_user(const vdf::object *user_tree)
	: steamid(std::stoull(user_tree->key))
	, account_name(user_tree->at("AccountName")->val())
	, persona_name(user_tree->at("PersonaName")->val())
	, remember_password(user_tree->at("RememberPassword")->val() != "0" ? true : false)
	, most_recent(user_tree->at("MostRecent")->val() != "0" ? true : false)
	, timestamp(std::stoll(user_tree->at("Timestamp")->val()))
{
}