#pragma once

# include "Libs.hpp"

class Client;

class Channel {
	private:
		int								_client_max;
		int								_client_count;
		std::string						_name;
		std::string						_topic;
		std::string						_password;
		std::vector<std::string>		_invite_list;
		std::map<std::string, bool>		_modes;
		std::map<std::string, Client*>	_clients;
		std::map<std::string, Client*>	_cmd_input;

	public:
		Channel(const std::string& name);
		~Channel();
		// setter
		void							setMode(std::string mode, bool value);
		void							setName(const std::string& name);
		void							setPassword(std::string& password);
		void							setTopic(const std::string& topic);
		void							setMaxClient(int limit);
		//getters
		int								getUserCount() const;
		int								getUserLimit() const;
		bool							getMode(std::string mode) const;
		std::vector<Client*>			getAllClients() const;
		std::string 					getName() const;
		std::string						getPassword() const;
		std::string						getTopic() const;
		std::string						getAllModes() const;
		std::map<std::string, Client*>	getOperators() const;
		std::map<std::string, Client*>	getNonOperators() const;
		std::string						getFormattedClientList() const;
		// methods functions
		void							clientIncrement(void);
		void							clientDecrement(void);
		bool							checkInvite(const std::string _nickname) const;
		bool							checkOperator(const std::string _nickename) const;
		bool							checkOnChannel(const std::string _nickname) const;
		bool							removeClient(const std::string& _nickname);
		void							addClient(Client* _client);
		void							addInvList(const std::string _nickname);
		void 							removeInvList(const std::string _nickname);
		void							promoteClient(std::string _nickname);
		void							downGradeClient(std::string _nickname);
		void							listClient() const;
		void							listOperators() const;
};
