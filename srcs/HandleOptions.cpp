/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleOptions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:53:26 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/13 15:39:27 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

std::vector<std::string>	Server::tokenizeInput(std::string _command)
{
	std::string 				_word;
	std::stringstream 			_ss(_command);
	std::vector<std::string> 	_tokens;

	while (std::getline(_ss, _word, ' ')) {
		if (_word.find('\r') != std::string::npos)
			_tokens.push_back(_word.substr(0, _word.find('\r')));
		else
			_tokens.push_back(_word);
	}
	if (_tokens.size() == 0)
		_tokens.push_back("");
	return (_tokens);
};

std::vector<std::string> Server::tokenize_buffer(std::string _buffer)
{
	std::vector<std::string> _tokens;
	std::string _word;
	std::stringstream _ss(_buffer);
	
	while (!_ss.eof() && std::getline(_ss, _word, '\n'))
		_tokens.push_back(_word);
	return (_tokens);
};

void	Server::sendUnknowCommand(std::string _command, int _client_fd)
{
	std::string _response = "Unknow command: " + _command + "\n";
	if (send(_client_fd, _response.c_str(), _response.size(), 0) < 0)
		std::cerr << "Error sending message" << std::endl;
};

void	Server::setupOptions(std::string& _buffer, int _client_fd)
{
	std::vector<std::string> _split_tokens = tokenize_buffer(_buffer);
	Client* _client = Server::handleClientFd(_client_fd);
	
	std::string _requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "MODE", "TOPIC", "INVITE", "KICK", "WHO", "PASS", "PART"};
	
	do {
		int i = 0;
		std::string _options = _split_tokens[0].substr(0, _split_tokens[0].find(' '));
		std::cout << "Buffer: " << _buffer << std::endl;
		std::cout << "Client: " << _client_fd << std::endl;

		for (; i < 13; i++)
		{
			if (_options == _requests[i])
				break;
		}
		if (_client->isAuthorized() == false)
		{
			if (i != 0 && i != 1 && i != 2 && i != 11) {
				std::string _response = "You must be authorized to use this command\n";
				send(_client_fd, _response.c_str(), _response.size(), 0);
				_client->_buffer_client.clear();
				return ;
			}
		}
		std::string _parse_cmd = _split_tokens[0].substr(_split_tokens[0].find(' ') + 1);
		switch (i)
		{
			case 0:
				cap(_client_fd);
				break ;
			case 1:
				user(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 2:
				nick(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 3:
				join(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 4:
				privmsg(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 5:
				_client->_buffer_client.clear();
				quit(tokenizeInput(_parse_cmd), _client_fd);
				_split_tokens.erase(_split_tokens.begin());
				return ;
				break ;
			case 6:
				mode(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 7:
				topic(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 8:
				invite(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 9:
				kick(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 10:
				who(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			case 11:
				if (pass(tokenizeInput(_parse_cmd), _client_fd) == false)
					return ;
				break ;
			case 12:
				part(tokenizeInput(_parse_cmd), _client_fd);
				break ;
			default:
				sendUnknowCommand(_options, _client_fd);
				break ;
		}
		_split_tokens.erase(_split_tokens.begin());
	}
	while (!_split_tokens.empty());
	_client->_buffer_client.clear();
};