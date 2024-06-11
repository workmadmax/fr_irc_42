/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 10:22:47 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:13 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::invite(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string	_response;
	Client* _client = Server::handleClientFd(_client_fd);
	
	if (_cmd_args.size() == 0 || _cmd_args[0] == "") {
		_response = "ERROR :Invalid number of arguments";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	const std::string& _channel_name = _cmd_args[1];
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
	
	std::istringstream 	_iss(_cmd_args[0]);
	std::string			_inv_nick;

	while (std::getline(_iss, _inv_nick, ',')) {
		Client* _inv_client = findClientByNick(_inv_nick);
		if (_inv_client == NULL) {
			_response = "ERROR :Client does not exist";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}
		if (_channel_ptr->checkOnChannel(_inv_nick)) {
			_response = "ERROR :Client is already in the channel";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}
		if (!_channel_ptr->checkOperator(_client->getNick())) {
			_response = "ERROR :You are not an operator";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}

		_channel_ptr->addInvList(_inv_nick);
		_response = "INVITE " + _inv_nick + " " + _channel_name;
		send(_client_fd, _response.c_str(), _response.size(), 0);

		_response = "INVITE " + _inv_nick + " " + _channel_name;
		int	inv_fd = _inv_client->getClientFd();
		send(inv_fd, _response.c_str(), _response.size(), 0);

		std::map<std::string, Client*> _clients = _channel_ptr->getOperators();
		for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			_response = ":" + _client->getNick() + " INVITE " + _inv_nick + " " + _channel_name;
			send(it->second->getClientFd(), _response.c_str(), _response.size(), 0);
		}
	}
};