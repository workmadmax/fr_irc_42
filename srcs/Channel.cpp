/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 11:39:17 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 17:57:42 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Channel.hpp"


/**
 * @brief Initializes the modes of a channel.
 * 
 * This function takes a reference to a map of string-bool pairs representing the modes of a channel.
 * It sets the initial values of the modes by assigning false to "o", true to "t", and false to "k", "l", and "i".
 * 
 * @param _modes The map of string-bool pairs representing the modes of a channel.
 */
void	initModes(std::map<std::string, bool>& _modes)
{
	_modes["o"] = false;
	_modes["t"] = true;
	_modes["k"] = false;
	_modes["l"] = false;
	_modes["i"] = false;
};

/**
 * @brief Constructs a Channel object with the given name.
 *
 * This constructor initializes the Channel object with the provided name and sets the initial values for other member variables.
 *
 * @param name The name of the channel.
 */
Channel::Channel(const std::string& name) : _name(name)
{
	initModes(_modes);
	_client_max = _MAX_CLIENT;
	_client_count = 0;
	_topic = "";
	_password = "";
};

/**
 * @brief Destructor for the Channel class.
 * 
 * This destructor clears the list of clients and the command input buffer.
 */
Channel::~Channel()
{
	_clients.clear();
	_cmd_input.clear();
};

// setters

/**
 * @brief Sets the mode of the channel.
 *
 * This function sets the mode of the channel specified by the given mode string.
 * The mode can be enabled or disabled based on the value parameter.
 *
 * @param mode The mode string representing the channel mode.
 * @param value The value indicating whether to enable or disable the mode.
 */
void Channel::setMode(std::string mode, bool value)
{
	_modes[mode] = value;
};

/**
 * @brief Sets the name of the channel.
 *
 * This function sets the name of the channel to the specified name.
 *
 * @param name The name of the channel.
 */
void Channel::setName(const std::string& name)
{
	this->_name = name;
};

/**
 * @brief Set the password for the channel.
 *
 * This function sets the password for the channel. The password is used to restrict access to the channel.
 *
 * @param password The password to set for the channel.
 */
void Channel::setPassword(std::string& password)
{
	_password = password;
};

/**
 * @brief Sets the topic of the channel.
 *
 * This function sets the topic of the channel to the specified topic.
 *
 * @param topic The topic to set for the channel.
 */
void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
};

/**
 * @brief Sets the maximum number of clients allowed in the channel.
 * 
 * This function sets the maximum number of clients that can join the channel. 
 * Once the maximum limit is reached, no more clients can join the channel until 
 * the limit is increased or clients leave the channel.
 * 
 * @param limit The maximum number of clients allowed in the channel.
 */
void Channel::setMaxClient(int limit)
{
	_client_max = limit;
};

// getters

/**
 * @brief Get the number of users in the channel.
 * 
 * @return The number of users in the channel.
 */
int Channel::getUserCount() const
{
	return (_client_count);
};

/**
 * @brief Get the user limit of the channel.
 *
 * This function returns the maximum number of users allowed in the channel.
 *
 * @return The user limit of the channel.
 */
int Channel::getUserLimit() const
{
	return (_client_max);
};

/**
 * @brief Retrieves the mode of the channel.
 *
 * This function returns the mode of the channel specified by the given mode string.
 *
 * @param mode The mode string representing the channel mode.
 * @return The mode of the channel.
 */
bool Channel::getMode(std::string mode) const
{
	return (_modes.at(mode));
};

/**
 * @brief Retrieves all clients associated with the channel.
 * 
 * This function returns a vector of pointers to Client objects that are associated with the channel.
 * It includes both the clients stored in the "_clients" map and the clients stored in the "_cmd_input" map.
 * 
 * @return std::vector<Client*> A vector of pointers to Client objects.
 */
std::vector<Client*> Channel::getAllClients() const
{
	std::vector<Client*> _clients_vector;
	
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		_clients_vector.push_back(it->second);
	for (std::map<std::string, Client*>::const_iterator it = _cmd_input.begin(); it != _cmd_input.end(); ++it)
		_clients_vector.push_back(it->second);
	return (_clients_vector);
};

/**
 * @brief Get the name of the channel.
 * 
 * @return std::string The name of the channel.
 */
std::string Channel::getName() const
{
	return (this->_name);
};

/**
 * @brief Returns the password of the channel.
 * 
 * @return The password of the channel.
 */
std::string Channel::getPassword() const
{
	return (this->_password);
};

/**
 * @brief Get the topic of the channel.
 * 
 * @return std::string The topic of the channel.
 */
std::string Channel::getTopic() const
{
	return (this->_topic);
};

/**
 * @brief Get all modes of the channel.
 * 
 * This function returns a string containing all the modes of the channel.
 * The modes are represented by characters and are prefixed with a '+' sign.
 * The available modes are 'o', 't', 'k', 'l', and 'i'.
 * 
 * @return std::string The string containing all the modes of the channel.
 */
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
/**
 * @brief Retrieves the map of operators associated with the channel.
 * 
 * This function returns a map of string-Client* pairs representing the operators associated with the channel.
 * The map contains the clients stored in the "_cmd_input" map, where the key is the nickname of the client and the value is a pointer to the Client object.
 * 
 * @return std::map<std::string, Client*> A map of string-Client* pairs representing the operators associated with the channel.
 */
std::map<std::string, Client*> Channel::getOperators() const
{
	return (this->_cmd_input);
};

/**
 * @brief Returns a map of non-operator clients in the channel.
 *
 * This function returns a map containing all the non-operator clients in the channel.
 *
 * @return A map of non-operator clients in the channel.
 */
std::map<std::string, Client*> Channel::getNonOperators() const
{
	return (this->_clients);
};

/**
 * @brief Returns a formatted string containing the list of clients in the channel.
 *
 * This function iterates over the `_clients` and `_cmd_input` maps and appends the names of the clients to a string, separated by spaces.
 * The resulting string represents the list of clients in the channel.
 *
 * @return A string containing the formatted list of clients in the channel.
 */
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

/**
 * @brief Increments the client count of the channel.
 */
void Channel::clientIncrement(void)
{
	_client_count++;
};

/**
 * @brief Decrements the client count of the channel.
 *
 * This function decrements the client count of the channel by one.
 */
void Channel::clientDecrement(void)
{
	_client_count--;
};

/**
 * @brief Checks if a nickname is in the invite list of the channel.
 * 
 * @param _nickname The nickname to check.
 * @return true if the nickname is in the invite list, false otherwise.
 */
bool	Channel::checkInvite(const std::string _nickname) const
{
	for (std::vector<std::string>::const_iterator it = _invite_list.begin(); it != _invite_list.end(); ++it)
	{
		if (*it == _nickname)
			return (true);
	}
	return (false);
};

/**
 * @brief Checks if a nickname is an operator in the channel.
 * 
 * @param _nickname The nickname to check.
 * @return true if the nickname is an operator, false otherwise.
 */
bool	Channel::checkOperator(const std::string _nickname) const
{
	if (_cmd_input.find(_nickname) != _cmd_input.end())
		return (true);
	return (false);
};

/**
 * @brief Checks if a given nickname is present on the channel.
 * 
 * This function checks if the specified nickname is present on the channel. It searches for the nickname in both the client list and the operator list of the channel.
 * 
 * @param _nickname The nickname to check.
 * @return true if the nickname is present on the channel, false otherwise.
 */
bool	Channel::checkOnChannel(const std::string _nickname) const
{
	bool	_client_check_channel = (_clients.find(_nickname) != _clients.end());
	bool	_operator_check_channel = (_cmd_input.find(_nickname) != _cmd_input.end());

	return (_client_check_channel || _operator_check_channel);
};

/**
 * @brief Removes a client from the channel.
 *
 * This function removes the client with the specified nickname from the channel.
 * If the client is found and successfully removed, the client count is decremented and true is returned.
 * If the client is not found, an error message is printed and false is returned.
 *
 * @param _nickname The nickname of the client to remove.
 * @return true if the client is successfully removed, false otherwise.
 */
bool Channel::removeClient(const std::string& _nickname)
{
	std::map<std::string, Client*>::iterator it = _clients.find(_nickname);

	if (it != _clients.end())
	{
		_clients.erase(it);
		clientDecrement();
		return true;
	}
	else
	{
		std::cout << "Client not found in channel" << std::endl;
		return false;
	}
};

/**
 * @brief Adds a client to the channel.
 * 
 * This function adds a client to the channel by inserting it into the _clients map.
 * The client is inserted using its nickname as the key and the client object as the value.
 * After adding the client, the client count is incremented.
 * 
 * @param _Client A pointer to the client object to be added.
 */
void	Channel::addClient(Client* _Client)
{
	_clients.insert(std::make_pair(_Client->getNick(), _Client));
	clientIncrement();
};

/**
 * @brief Adds a nickname to the invite list of the channel.
 * 
 * This function adds the specified nickname to the invite list of the channel.
 * The invite list is used to keep track of nicknames that are allowed to join the channel.
 * 
 * @param _nickname The nickname to be added to the invite list.
 */
void	Channel::addInvList(const std::string _nickname)
{
	_invite_list.push_back(_nickname);
};

/**
 * @brief Removes a nickname from the invite list of the channel.
 * 
 * This function removes the specified nickname from the invite list of the channel.
 * If the nickname is found in the invite list, it is removed and the function returns.
 * If the nickname is not found, the function does nothing.
 * 
 * @param _nickname The nickname to be removed from the invite list.
 */
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

/**
 * @brief Promotes a client to a higher privilege level within the channel.
 * 
 * This function moves a client from the `_cmd_input` map to the `_clients` map,
 * effectively promoting them to a higher privilege level within the channel.
 * 
 * @param _nickname The nickname of the client to be promoted.
 */
void	Channel::promoteClient(std::string _nickname)
{
	std::map<std::string, Client*>::iterator it = _cmd_input.find(_nickname);
	
	if (it != _cmd_input.end())
	{
		_clients.insert(std::make_pair(it->first, it->second));
		_cmd_input.erase(it);
	}
};

/**
 * @brief Downgrades a client in the channel.
 *
 * This function moves a client from the main client list to the command input list.
 * The client is identified by their nickname.
 *
 * @param _nickname The nickname of the client to be downgraded.
 */
void	Channel::downGradeClient(std::string _nickname)
{
	std::map<std::string, Client*>::iterator it = _clients.find(_nickname);
	
	if (it != _clients.end())
	{
		_cmd_input.insert(std::make_pair(it->first, it->second));
		_clients.erase(it);
	}
};

/**
 * @brief Lists all the clients in the channel.
 *
 * This function prints the names of all the clients currently present in the channel.
 */
void	Channel::listClient() const
{
	std::cout << "Clients: \n";
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		std::cout << it->first << std::endl;
};

/**
 * @brief Lists the operators in the channel.
 *
 * This function prints the client file descriptors of all the operators in the channel.
 */
void	Channel::listOperators() const
{
	std::cout << "Operators: \n";
	for (std::map<std::string, Client*>::const_iterator it = _cmd_input.begin(); it != _cmd_input.end(); ++it)
		std::cout << it->second->getClientFd() << std::endl;
};