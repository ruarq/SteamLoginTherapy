#include "steam_user_database.hpp"

auto steam_user_database::load(const std::string &filename) -> bool
{
	auto loginusers = vdf::read_file(filename);
	
	if (loginusers)
	{
		auto result = load(vdf::read_file(filename));
		delete loginusers;
		return result;
	}
	else
	{
		return false;
	}
}

auto steam_user_database::load(const vdf::object *loginusers_vdf) -> bool
{
	if (!loginusers_vdf)
	{
		return false;
	}

	auto users = loginusers_vdf->at("users");
	if (!users)
	{
		return false;
	}

	for (auto user : *users)
	{
		if (!user)
		{
			return false;
		}

		this->users.push_back(steam_user(user));
	}

	return true;
}

auto steam_user_database::save(const std::string &filename) const -> bool
{
	const auto vdf = get_vdf();
	
	if (vdf)
	{
		auto result = vdf::write_file(filename, vdf);
		delete vdf;
		return result;
	}
	else
	{
		return false;
	}
}

auto steam_user_database::get_vdf() const -> vdf::object*
{
	auto wrapper = new vdf::list();
	auto loginusers = new vdf::list();
	loginusers->key = "users";
	wrapper->push_back(loginusers);

	for (const auto &user : users)
	{
		loginusers->push_back(user.to_vdf());
	}

	return wrapper;
}

auto steam_user_database::query_all_users() -> std::vector<steam_user>&
{
	return users;
}

auto steam_user_database::query(const steamid_t steamid) -> steam_user*
{
	for (auto &user : users)
	{
		if (user.steamid == steamid)
		{
			return &user;
		}
	}

	return nullptr;
}

auto steam_user_database::query(const std::string &name, const steam_user_database_query_type query_selection) -> steam_user*
{
	for (auto &user : users)
	{
		if (query_selection == ACCOUNT_NAME)
		{
			if (user.account_name == name)
			{
				return &user;
			}
		}
		else if (query_selection == PERSONA_NAME)
		{
			if (user.persona_name == name)
			{
				return &user;
			}
		}
		else
		{
			break;
		}
	}

	return nullptr;
}

auto steam_user_database::query_most_recent() -> steam_user*
{
	for (auto &user : users)
	{
		if (user.most_recent)
		{
			return &user;
		}
	}

	return nullptr;
}