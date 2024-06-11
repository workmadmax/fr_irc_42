/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:48:44 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:54:02 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::part(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string		_response;
	std::string		_party;
	Client* _client = Server::handleClientFd(_client_fd);
	
	if (_cmd_args.empty()) {
		_response = "ERROR :Invalid number of arguments";
		return ;
	} else if (_cmd_args.size() > 1) {
		for (size_t i = 1; i < _cmd_args.size(); i++)
			_party += _cmd_args[i];
	}

	std::stringstream		_iss(_cmd_args[0]);
	std::string				_channel_name;
	
	while (std::getline(_iss, _channel_name, ','))
	{
		Channel* _channel_ptr = getChannel(_channel_name);
		if (_channel_ptr == NULL)
		{
			_response = "ERROR :Channel does not exist";
			send(_client_fd, _response.c_str(), _response.size(), 0);
		}
		else
		{
			_response = ":" + _client->getNick() + "!" + _client->getUsername() + "@" + _client->getHostname() + " PART " + _channel_name + "\r\n";
			std::vector<Client*> _clients = _channel_ptr->getAllClients();
			for (size_t i = 0; i < _clients.size(); i++)
				send(_clients[i]->getClientFd(), _response.c_str(), _response.size(), 0);
		}
		_channel_ptr->removeClient(_client->getNick());
		if (_channel_ptr->getAllClients().empty())
			removeChannel(_channel_name);
	}
};