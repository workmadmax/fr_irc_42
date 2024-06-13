/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:31:54 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/13 16:01:58 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"

Client::Client(const int _client_fd) : _client_fd(_client_fd), _nickname(""), _username(""), _hostname(""), _password("")
{
};

int		Client::getClientFd()
{
	return (_client_fd);
};

bool	Client::isAuthorized()
{
	if (this->_username.empty() || this->_nickname.empty() || this->_password.empty())
		return (false);
	return (true);
};

std::string	Client::getNick()
{
	return (_nickname);
};

std::string	Client::getUsername()
{
	return (_username);
};

std::string	Client::getHostname()
{
	return (_hostname);
};

std::string	Client::getPassword()
{
	return (_password);
};

std::string	Client::getIP()
{
	return (_client_IP);
};

// setters

void	Client::setNick(std::string nick)
{
	_nickname = nick;
};

void	Client::setUsername(std::string username)
{
	_username = username;
};

void	Client::setHostname(std::string hostname)
{
	_hostname = hostname;
};

void	Client::setPassword(std::string password)
{
	_password = password;
};

void	Client::setIP(std::string ip)
{
	_client_IP = ip;
};