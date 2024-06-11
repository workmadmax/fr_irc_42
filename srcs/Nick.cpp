/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 10:05:07 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/10 10:18:54 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::nick(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string	_response;
	
	if (_cmd_args.size() < 1)
	{
		_response = "ERROR :Invalid number of arguments";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	Client*	_client = Server::handleClientFd(_client_fd);
	if (_client == NULL)
		return ;
	std::string _old_nick = _cmd_args[0].substr(0, _cmd_args[0].find('\r'));
	if (_old_nick.empty())
	{
		_response = "ERROR :Invalid nickname";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	if (Server::findClientByNick(_old_nick) != NULL)
	{
		_response = "ERROR :Nickname is already in use";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	_client->setNick(_old_nick);
	if (_client->isAuthorized() == true)
	{
		_response = ": Welcome to the server " + _old_nick;
		send(_client_fd, _response.c_str(), _response.size(), 0);
	}
};