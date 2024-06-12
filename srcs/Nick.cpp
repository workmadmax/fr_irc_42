/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 10:05:07 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 18:47:37 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::nick(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string	_response;
	std::string _old_nick = _cmd_args[0].substr(0, _cmd_args[0].find('\r'));
	
	if (_cmd_args.size() < 1)
	{
		_response = "461 " + _old_nick + " :Not enough parameters";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	Client*	_client = Server::handleClientFd(_client_fd);
	if (_client == NULL)
		return ;
	if (_old_nick.empty())
	{
		_response = "432 " + _old_nick + " :Erroneous nickname";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	if (Server::findClientByNick(_old_nick) != NULL)
	{
		_response = "433 " + _old_nick + " :Nickname is already in use";
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return ;
	}
	_client->setNick(_old_nick);
	if (_client->isAuthorized() == true)
	{
		_response = ":" + _old_nick + "!~" + _old_nick + "@localhost" + " JOIN :" + _old_nick;
		send(_client_fd, _response.c_str(), _response.size(), 0);
	}
};