/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 10:12:44 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:54:57 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

void	Server::who(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string			_response;
	std::string			_channel_name;
	Channel*			_channel_ptr;

	_channel_name = _cmd_args[0];
	_channel_ptr = getChannel(_channel_name);
	std::vector<Client* > _clients = _channel_ptr->getAllClients();

	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::string _flag = "H";
		if (_channel_ptr->checkOperator(_clients[i]->getNick()))
			_flag += "@ ";
		_response = ":" + _clients[i]->getNick() + " " + _flag + " " + _channel_name + "\n";
		std::cout << _response << std::endl;
		if (send(_client_fd, _response.c_str(), _response.size(), 0) == -1)
			throw std::runtime_error("Error: send");
	}
	_response = "End of /WHO list";
	if (send(_client_fd, _response.c_str(), _response.size(), 0) == -1)
		throw std::runtime_error("Error: send"); 
};