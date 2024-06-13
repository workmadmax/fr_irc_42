/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 10:07:59 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/13 12:49:20 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::user(std::vector<std::string> _cmd_args, int _client_fd)
{
	Client* _client = Server::handleClientFd(_client_fd);
	if (_client == NULL)
		return ;
	std::vector<Client*> _client_vec = getClients();
	std::string _response;
	std::string _username = _cmd_args[0].substr(0, _cmd_args[0].find_first_of(" "));

	for (std::vector<Client*>::iterator it = _client_vec.begin(); it != _client_vec.end(); it++) {
		if ((*it)->getUsername() == _username) {
			_response = "433 " + _username + " :Nickname is already in use\n";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}
	}
	if (!_client->getUsername().empty()) {
		_response = "462 :You may not reregister\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	_client->setUsername(_username);
	if (_cmd_args.size() > 3)
		_client->setHostname(_cmd_args[3].substr(1, _cmd_args[3].find_first_of('\r')));
	
	if (_client->isAuthorized() == true) {
		_response = "001 " + _client->getUsername() + " :Welcome to the Internet Relay Network " + _client->getHostname() + "!" + _client->getHostname() + "@" + _client->getHostname() + "\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
};