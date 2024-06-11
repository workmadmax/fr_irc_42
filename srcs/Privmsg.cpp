/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:17:36 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:51 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

static std::string	formattedString(std::vector<std::string> _cmd_args)
{
	std::string		_formatted_string;
	
	if (_cmd_args[1].find(':') == 0) {
		_formatted_string = _cmd_args[1].substr(1) + " ";
		for (size_t i = 2; i < _cmd_args.size(); i++) {
			_formatted_string += _cmd_args[i] + " ";
		}
		return (_formatted_string);
	}
	_formatted_string = _cmd_args[1].substr(0) + " ";
	for (size_t i = 2; i < _cmd_args.size(); i++) {
		_formatted_string += _cmd_args[i] + " ";

	}
	return (_formatted_string);
};

void	Server::privmsg(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string		_response;
	Client* _client = Server::handleClientFd(_client_fd);
	std::string _channel_name = _cmd_args[0].substr(0, _cmd_args[0].find(' '));

	if (_cmd_args.size() < 1 || _channel_name.empty())
	{
		_response = ":localhost 461 " + _client->getNick() + " PRIVMSG :Not enough parameters";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}

	if (_cmd_args.size() < 2 || _cmd_args[1].empty())
	{
		_response = ":localhost 412 " + _client->getNick() + " PRIVMSG :No text to send";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}

	std::string _formatted_string = formattedString(_cmd_args);
	std::cout << "Sending message to channel: " << _channel_name << std::endl;
	std::cout << "PRIVMSG: " << _formatted_string << std::endl;

	Channel* _channel_ptr = getChannel(_channel_name);
	if (_channel_ptr == NULL && _channel_name[0] == '#')
	{
		_response = ":localhost 401 " + _client->getNick() + " " + _channel_name + " :No such nick/channel";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	else if (_channel_name[0] != '#')
	{
		Client* _receiver = findClientByNick(_channel_name);
		if (_receiver == NULL)
		{
			_response = ":localhost 401 " + _client->getNick() + " " + _channel_name + " :No such nick/channel";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}
		_response = ":" + _client->getNick() + " PRIVMSG " + _receiver->getNick() + " :" + _formatted_string;
		std::cout << "Sending message to client: " << _receiver->getNick() << std::endl;
		send(_receiver->getClientFd(), _response.c_str(), _response.size(), 0);
		return ;
	}

	_response = ":" + _client->getNick() + " PRIVMSG " + _channel_name + " :" + _formatted_string;
	std::cout << "Sending message to channel: " << _channel_name << std::endl;
	std::map<std::string, Client*> _clients = _channel_ptr->getNonOperators();
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second && it->second->getClientFd() != _client_fd)
			send(it->second->getClientFd(), _response.c_str(), _response.size(), 0);
	}
};