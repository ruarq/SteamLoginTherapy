#include "steam_user.hpp"

steam_user::steam_user(const vdf::object *user_vdf)
{
	from(user_vdf);
}

auto steam_user::from(const vdf::object *user_vdf) -> bool
{
	if (!user_vdf)
	{
		DEBUG_LOG("user_vdf is invalid");
		return false;
	}

	bool success = true;

	try
	{
		steamid = std::stoull(user_vdf->key);

		if (auto account_name = user_vdf->at("AccountName"))
		{
			this->account_name = account_name->val();
		}
		else
		{
			DEBUG_LOG("account_name is invalid");
			success = false;
		}

		if (auto persona_name = user_vdf->at("PersonaName"))
		{
			this->persona_name = persona_name->val();
		}
		else
		{
			DEBUG_LOG("persona_name is invalid");
			success = false;
		}

		if (auto remember_password = user_vdf->at("RememberPassword"))
		{
			this->remember_password = remember_password->val() != "0";
		}
		else
		{
			DEBUG_LOG("remember_password is invalid");
			success = false;
		}

		if (auto most_recent = user_vdf->at("MostRecent"))
		{
			this->most_recent = most_recent->val() != "0";
		}
		else
		{
			DEBUG_LOG("most_recent is invalid");
			success = false;
		}

		if (auto timestamp = user_vdf->at("Timestamp"))
		{
			this->timestamp = std::stoull(timestamp->val());
		}
		else
		{
			DEBUG_LOG("timestamp is invalid");
			success = false;
		}
	}
	catch (std::exception &e)
	{
		DEBUG_LOG(e.what());
		success = false;
	}

	return true;
}

auto steam_user::to_vdf() const -> vdf::object*
{
	auto vdf = new vdf::list();
	vdf->key = std::to_string(steamid);
	vdf->push_back(new vdf::value("AccountName", account_name));
	vdf->push_back(new vdf::value("PersonaName", persona_name));
	vdf->push_back(new vdf::value("RememberPassword", remember_password ? "1" : "0"));
	vdf->push_back(new vdf::value("MostRecent", most_recent ? "1" : "0"));
	vdf->push_back(new vdf::value("Timestamp", std::to_string(timestamp)));
	return vdf;
}