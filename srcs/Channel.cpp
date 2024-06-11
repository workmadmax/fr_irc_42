/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:39:17 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/09 10:17:54 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Channel.hpp"

Channel::Channel(const std::string& name) : _name(name)
{
	_client_max = 10;
	_client_count = 0;
	_topic = "";
	_password = "";
};

Channel::~Channel()
{
	_clients.clear();
	_cmd_input.clear();
};

// setters

void Channel::setMode(std::string mode, bool value)
{
	_modes[mode] = value;
};

void Channel::setName(const std::string& name)
{
	this->_name = name;
};

void Channel::setPassword(std::string& password)
{
	_password = password;
};

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
};

void Channel::setMaxClient(int limit)
{
	_client_max = limit;
};

// getters

int Channel::getUserCount() const
{
	return (_client_count);
};

int Channel::getUserLimit() const
{
	return (_client_max);
};

bool Channel::getMode(std::string mode) const
{
	return (_modes.at(mode));
};

std::vector<Client*> Channel::getAllClients() const
{
	std::vector<Client*> _clients_vector;
	
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		_clients_vector.push_back(it->second);
	for (std::map<std::string, Client*>::const_iterator it = _cmd_input.begin(); it != _cmd_input.end(); ++it)
		_clients_vector.push_back(it->second);
	return (_clients_vector);
};

std::string Channel::getName() const
{
	return (this->_name);
};

std::string Channel::getPassword() const
{
	return (this->_password);
};

std::string Channel::getTopic() const
{
	return (this->_topic);
};

std::string Channel::getAllModes() const
{
	std::string modes = "+";
	if (_modes.at("o")) modes += "o";
	if (_modes.at("t")) modes += "t";
	if (_modes.at("k")) modes += "k";
	if (_modes.at("l")) modes += "l";
	if (_modes.at("i")) modes += "i";
	return (modes);	
};

std::map<std::string, Client*> Channel::getOperators() const
{
	return (this->_cmd_input);
};

std::map<std::string, Client*> Channel::getNonOperators() const
{
	return (this->_clients);
};

std::string Channel::getFormattedClientList() const
{
	std::string users_list;
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		users_list += it->first + " ";
	for (std::map<std::string, Client*>::const_iterator it = _cmd_input.begin(); it != _cmd_input.end(); ++it)
		users_list += it->first + " ";
	return (users_list);
};

// methods functions

void Channel::clientIncrement(void)
{
	_client_count++;
};

void Channel::clientDecrement(void)
{
	_client_count--;
};

bool	Channel::checkInvite(const std::string _nickname) const
{
	for (std::vector<std::string>::const_iterator it = _invite_list.begin(); it != _invite_list.end(); ++it)
	{
		if (*it == _nickname)
			return (true);
	}
	return (false);
};

bool	Channel::checkOperator(const std::string _nickname) const
{
	if (_cmd_input.find(_nickname) != _cmd_input.end())
		return (true);
	return (false);
};

bool	Channel::checkOnChannel(const std::string _nickname) const
{
	bool	_client_check_channel = (_clients.find(_nickname) != _clients.end());
	bool	_operator_check_channel = (_cmd_input.find(_nickname) != _cmd_input.end());

	return (_client_check_channel || _operator_check_channel);
};

bool	Channel::removeClient(const std::string& _nickname)
{
	std::map<std::string, Client*>::iterator it = _clients.find(_nickname);
	
	if (it != _clients.end())
	{
		_clients.erase(it);
		clientDecrement();
		return (true);
	} else {
		std::cout << "Client not found in channel" << std::endl;
		return (false);
	}
};

void	Channel::addClient(Client* _Client)
{
	_clients.insert(std::make_pair(_Client->getNick(), _Client));
	clientIncrement();
};

void	Channel::addInvList(const std::string _nickname)
{
	_invite_list.push_back(_nickname);
};

void	Channel::removeInvList(const std::string _nickname)
{
	for (std::vector<std::string>::iterator it = _invite_list.begin(); it != _invite_list.end(); ++it)
	{
		if (*it == _nickname)
		{
			_invite_list.erase(it);
			return ;
		}
	}
};

void	Channel::promoteClient(std::string _nickname)
{
	std::map<std::string, Client*>::iterator it = _cmd_input.find(_nickname);
	
	if (it != _cmd_input.end())
	{
		_clients.insert(std::make_pair(it->first, it->second));
		_cmd_input.erase(it);
	}
};

void	Channel::downGradeClient(std::string _nickname)
{
	std::map<std::string, Client*>::iterator it = _clients.find(_nickname);
	
	if (it != _clients.end())
	{
		_cmd_input.insert(std::make_pair(it->first, it->second));
		_clients.erase(it);
	}
};

void	Channel::listClient() const
{
	std::cout << "Clients: \n";
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		std::cout << it->first << std::endl;
};

void	Channel::listOperators() const
{
	std::cout << "Operators: \n";
	for (std::map<std::string, Client*>::const_iterator it = _cmd_input.begin(); it != _cmd_input.end(); ++it)
		std::cout << it->second->getClientFd() << std::endl;
};