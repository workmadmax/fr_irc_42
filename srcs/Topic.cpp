/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 11:41:13 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:44:55 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

std::vector<std::string> _tokenize_buffer(std::string _buffer)
{
	std::vector<std::string> _cmd_args;
	
	size_t _pos = _buffer.find(" ");
	if (_pos != std::string::npos)
	{
		std::string _word = _buffer.substr(0, _pos);
		std::string _rest = _buffer.substr(_pos + 1, _buffer.find("\r") - _pos - 1);
		_cmd_args.push_back(_word);
		_cmd_args.push_back(_rest);
	}
	else
		_cmd_args.push_back(_buffer);
	return _cmd_args;
}

void Server::topic(std::vector<std::string> _cmd_args, int _client_fd)
{
	Client* _client = findClientByNick(_cmd_args[0]);
	if (_client == NULL)
	{
		sendUnknowCommand("401", _client_fd);
		return;
	}
	if (_cmd_args.size() < 2)
	{
		sendUnknowCommand("461", _client_fd);
		return;
	}
	Channel* _channel = getChannel(_cmd_args[1]);
	if (_channel == NULL)
	{
		sendUnknowCommand("403", _client_fd);
		return;
	}
	if (_channel->checkOnChannel(_client->getNick()) == false)
	{
		sendUnknowCommand("442", _client_fd);
		return;
	}
	if (_cmd_args.size() == 2)
	{
		std::string _topic = _channel->getTopic();
		if (_topic.empty())
			sendUnknowCommand("331", _client_fd);
		else
		{
			std::string _response = "332 " + _channel->getName() + " :" + _topic + "\r\n";
			send(_client_fd, _response.c_str(), _response.size(), 0);
		}
	}
	else
	{
		if (_channel->checkOperator(_client->getNick()) == false)
		{
			sendUnknowCommand("482", _client_fd);
			return;
		}
		std::string _new_topic = _cmd_args[2];
		_channel->setTopic(_new_topic);
		std::string _response = "332 " + _channel->getName() + " :" + _new_topic + "\r\n";
		send(_client_fd, _response.c_str(), _response.size(), 0);
	}
};