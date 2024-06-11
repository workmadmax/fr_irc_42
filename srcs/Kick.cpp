/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:29:08 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:22 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::kick(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string _response;
	std::string	_kick;
	const std::string& _channel_name = _cmd_args[0];
	Client* _client = Server::handleClientFd(_client_fd);

	if (_cmd_args.size() == 0 || _cmd_args[0] == "") {
		_response = "ERROR :Invalid number of arguments";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	if (_cmd_args.size() == 3)
		_kick = _cmd_args[2].substr(1);
	else
		_kick = "not found";
	
	Channel* _channel_ptr = getChannel(_channel_name);
	if (_channel_ptr == NULL) {
		_response = "ERROR :Channel does not exist";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	
	if (!_channel_ptr->checkOnChannel(_client->getNick())) {
		_response = "ERROR :You are not in the channel";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}

	std::istringstream 	_iss(_cmd_args[1]);
	std::string			_nick_client;
	while (std::getline(_iss, _nick_client, '.')) {
		Client* _kick_client = findClientByNick(_nick_client);
		if (_kick_client == NULL) {
			_response = "ERROR :Client does not exist";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			continue ;
		}

		if (!_channel_ptr->checkOnChannel(_nick_client)) {
			_response = "ERROR :Client is not in the channel";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			continue ;
		}

		if (!_channel_ptr->checkOperator(_client->getNick())) {
			_response = "ERROR :You are not an operator";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}

		std::vector<Client *> _clients = _channel_ptr->getAllClients();
		_response = "KICK " + _channel_name + " " + _nick_client + " " + _kick;
		for (size_t i = 0; i < _clients.size(); i++) {
			send(_clients[i]->getClientFd(), _response.c_str(), _response.size(), 0);
		}
		_channel_ptr->removeClient(_nick_client);
	}
};