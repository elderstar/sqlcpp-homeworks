#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <Windows.h>
#include <vector>

class DBHandler
{
public:
	DBHandler(std::string host_, int port_, std::string dbname_,  std::string user_, std::string password_)
	{
			conn = std::make_unique<pqxx::connection>("host=" + host_ +
				" port=" + std::to_string(port_) +
				" dbname=" + dbname_ +
				" user=" + user_ +
				" password=" + password_);
			initTable();
			conn->prepare("insert_client", "INSERT INTO clients(first_name, last_name, email) VALUES($1, $2, $3) RETURNING id");
			conn->prepare("insert_phone", "INSERT INTO phones(phone, client_id) VALUES($1, $2)");
			conn->prepare("delete_client", "DELETE FROM clients WHERE id = $1");
			conn->prepare("delete_phone", "DELETE FROM phones WHERE id = $1");
			conn->prepare("delete_user_phones", "DELETE FROM phones WHERE client_id = $1");
			conn->prepare("select_user", "SELECT * FROM clients WHERE id = $1");
			conn->prepare("update_user", "UPDATE clients SET first_name=$2, last_name=$3, email=$4 WHERE id = $1");
			conn->prepare("find_user_by_fname", "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone FROM clients as c LEFT JOIN phones as p ON (p.client_id = c.id) WHERE c.first_name = $1");
			conn->prepare("find_user_by_lname", "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone FROM clients as c LEFT JOIN phones as p ON (p.client_id = c.id) WHERE c.last_name = $1");
			conn->prepare("find_user_by_email", "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone FROM clients as c LEFT JOIN phones as p ON (p.client_id = c.id) WHERE c.email = $1");
			conn->prepare("find_user_by_phone", "SELECT client_id FROM phones WHERE phone = $1");
			conn->prepare("find_user_by_id", "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone FROM clients as c LEFT JOIN phones as p ON (p.client_id = c.id) WHERE c.id = $1");
	};
	~DBHandler() 
	{
		if (conn->is_open()) {
			conn->close();
		}
	};

	void initTable() 
	{
		pqxx::work tx(*conn);
		tx.exec(
			"CREATE TABLE IF NOT EXISTS clients( id SERIAL NOT NULL, first_name VARCHAR(255) NOT NULL, "
			"last_name VARCHAR(255) NOT NULL, email VARCHAR(255) UNIQUE NOT NULL, PRIMARY KEY(id))"
		);
		tx.exec(
			"CREATE TABLE IF NOT EXISTS phones( "
			"id SERIAL NOT NULL, "
			"phone VARCHAR(255) NOT NULL, "
			"client_id int NOT NULL, "
			"PRIMARY KEY(id), "
			"FOREIGN KEY(client_id) REFERENCES clients(id) ON DELETE CASCADE ON UPDATE NO ACTION)"
		);
		tx.commit();
	}

	int registerClient(const std::string& first_name, const std::string& last_name, const std::string& email)
	{
		try
		{
			std::string last_inserted_id = "";
			pqxx::work tx(*conn);
			auto result = tx.exec_prepared("insert_client", first_name, last_name, email);
			
			tx.commit();
			return std::stoi(result.front()["id"].c_str());
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			return 0;
		}
	}

	void setPhoneToClient(std::vector<std::string>& phones, const int& client_id)
	{
		try
		{
			pqxx::work tx(*conn);

			for (const std::string& phone:phones)
			{
				tx.exec_prepared("insert_phone", phone, client_id);
			}
			tx.commit();
			phones.clear();
			phones.shrink_to_fit();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
	}

	bool deleteClient(const int& client_id)
	{
		try
		{
			pqxx::work tx(*conn);

			tx.exec_prepared("delete_client", client_id);
			tx.commit();
			return true;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			return false;
		}
	}

	bool deletePhone(const int& phone_id)
	{
		try
		{
			pqxx::work tx(*conn);

			tx.exec_prepared("delete_phone", phone_id);
			tx.commit();
			return true;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			return false;
		}
	}

	bool deleteClientsPhones(const int& client_id)
	{
		try
		{
			pqxx::work tx(*conn);

			tx.exec_prepared("delete_user_phones", client_id);
			tx.commit();
			return true;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			return false;
		}
	}
	
	pqxx::result findClient(const std::string& first_name, const std::string& last_name = "", const std::string& email = "", const std::string& phone = "")
	{
		try
		{
			pqxx::work tx(*conn);

			if (first_name != "")
			{
				return tx.exec_prepared("find_user_by_fname", first_name);
			}
			else if (last_name != "")
			{
				return tx.exec_prepared("find_user_by_lname", last_name);
			}
			else if (email != "")
			{
				return tx.exec_prepared("find_user_by_email", email);
			}
			else if (phone != "")
			{
				auto result = tx.exec_prepared("find_user_by_phone", phone);
				return tx.exec_prepared("find_user_by_id", result.front()["client_id"].c_str());
				
			}
			else {
				return tx.exec("");
			}
			tx.commit();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
	}

	bool updateClient(const int& client_id, const std::string& first_name = "", const std::string& last_name = "", const std::string& email = "")
	{
		try
		{
			pqxx::work tx(*conn);
			std::vector<std::string> client = {};

			auto result = tx.exec_prepared("select_user", client_id);

			first_name != "" ? client.push_back(first_name) : client.push_back(result.front()["first_name"].c_str());
			last_name != "" ? client.push_back(last_name) : client.push_back(result.front()["last_name"].c_str());
			email != "" ? client.push_back(email) : client.push_back(result.front()["email"].c_str());

			tx.exec_prepared("update_user", client_id, client.at(0), client.at(1), client.at(2));
			tx.commit();
			return true;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			return false;
		}
	}

private:
	std::unique_ptr<pqxx::connection> conn;
};

int main() 
{
	system("chcp 1251");
	setlocale(LC_ALL, "ru_RU.UTF-8");
	try
	{
		DBHandler db = DBHandler("localhost", 5432, "lesson5_table", "postgres", "postgres");

		std::string first_name = "";
		std::string last_name = "";
		std::string email = "";
		std::vector<std::string> phones = {};
		int id = 0;
		// имитирую введенные данные
		first_name = "Carlos";
		last_name = "Castaneda";
		email = "nonamewarrior@universe.net";
		// регистрирую клиента, и получаю его ID 
		id = db.registerClient(first_name, last_name, email);
		if (phones.size() > 0 && id != 0)
		{
			// если  телефон был введен, присваиваю телефоны клиенту
			db.setPhoneToClient(phones, id);
		}
		// имитирую введенные данные нового клиента с телефонами
		first_name = "Stephen";
		last_name = "Hawking";
		email = "singularityhacker@universe.net";
		phones = {};
		phones.push_back("18888888888");
		phones.push_back("17778887788");
		// регистрирую нового клиента
		id = db.registerClient(first_name, last_name, email);
		if (phones.size() > 0 && id != 0)
		{
			db.setPhoneToClient(phones, id);
		}
		// имитирую введенные данные 3его клиента с телефонами
		first_name = "John";
		last_name = "Flint";
		email = "treasureisland@goldenage.org";
		phones = {};
		phones.push_back("16669996699");
		phones.push_back("17778889966");
		// регистрирую 3его клиента
		id = db.registerClient(first_name, last_name, email);
		if (phones.size() > 0 && id != 0)
		{
			db.setPhoneToClient(phones, id);
		}
		//ищу клиента по имени
		auto result = db.findClient("John", "", "", "");
		//если с базы пришел не пустой результат, вывоже на экран, с телефонами, если они есть у клиента
		if (result.affected_rows() != 0)
		{
			id = std::stoi(result.front()["id"].c_str());
			std::cout << id << "\t" << result.front()["first_name"] << "\t" << result.front()["last_name"] << "\t" << result.front()["email"] << "\n";
			if (result.capacity() > 1)
			{
				for (auto row : result)
				{
					std::cout << "Phone #" << row["phone_id"] << ": " << row["phone"] << "\n";
				}
			}
		}

		db.updateClient(id, "Cap. John", "Flint");
		db.deleteClientsPhones(3);
		db.deleteClient(2);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
	return 0;
}
