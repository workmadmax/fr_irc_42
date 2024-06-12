/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 11:44:31 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 18:42:03 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::quit(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string	_quit_message;
	std::string	_client_nick = _cmd_args[0];
	Client*		_client = Server::handleClientFd(_client_fd);

	_quit_message = "ERROR :Closing Link: " + _client_nick + " (" + _client_nick + " Quit)";
	send(_client_fd, _quit_message.c_str(), _quit_message.size(), 0);
	removeClient(_client_fd);
	close(_client_fd);
	delete _client;
};