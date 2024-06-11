/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 10:07:59 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:59 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::user(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string		_response;
	std::string		_nickname = _cmd_args[0].substr(0, _cmd_args[0].find(' '));
	
	Client* _client = Server::handleClientFd(_client_fd);
	if (_client == NULL)
		return ;
	std::vector<Client *> _client_vec = getClients();
	
	for (std::vector<Client*>::iterator it = _client_vec.begin(); it != _client_vec.end(); ++it)
	{
		if ((*it)->getUsername() == _nickname)
		{
			_response = ":localhost 462 " + _client->getNick() + " USER :Already registered";
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return ;
		}
	}
	if (!_client->getUsername().empty())
	{
		_response = ":localhost 462 " + _client->getNick() + " USER :Already registered";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	_client->setUsername(_nickname);
	if (_cmd_args.size() > 3)
		_client->setHostname(_cmd_args[3].substr(1, _cmd_args[3].find('\r')));
	if (_client->isAuthorized() == true)
	{
		_response = ":localhost 001 " + _client->getNick() + " :Welcome to the Internet Relay Network " \
			+ _client->getNick() + "!" + _client->getUsername() + "@" + _client->getHostname();
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
};