/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:43:12 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 18:52:48 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

bool	Server::pass(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string _response;
	Client* _client = Server::handleClientFd(_client_fd);

	if (_cmd_args.size() != 1)
	{
		_response = "461 PASS :Not enough parameters\r\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return (false);
	}
	
	std::string _password = _cmd_args[0].substr(0, _cmd_args[0].find("\r"));
	if (!_client->getPassword().empty())
	{
		_response = "462 PASS: Password already set\r\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return (false);
	}
	
	_client->setPassword(_password);
	if (_client->getPassword() != Server::getPassword()) {
		_response = "464 :Invalid password\r\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		close(_client_fd);
		Client* _client = Server::handleClientFd(_client_fd);
		removeClient(_client_fd);
		delete _client;
		return (true);
	}
	return (false);
};