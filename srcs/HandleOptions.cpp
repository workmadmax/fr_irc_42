/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleOptions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:53:26 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:43:59 by mdouglas         ###   ########.fr       */
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
	std::vector<std::string> _tokens = tokenize_buffer(_buffer);
	Client* _client = Server::handleClientFd(_client_fd);
	std::string _requests[] = {
		"NICK", "USER", "JOIN", "PRIVAMSG", "TOPIC", \
		"MODE", "PRIVMSG", "QUIT", "WHO", "PASS", "KICK", "PART", "INVITE"
	};

	do {
		int i = 0;
		std::string _options = _tokens[0].substr(0, _tokens[0].find_first_of(" "));
		std::cout << "buffer: " << _buffer << std::endl;
		std::cout << "client: " << _client->getNick() << std::endl;
		for (; i < 13; i++) {
			if (_options == _requests[i])
				break;
		}
		if (_client->isAuthorized() == false) {
			if (i != 0 && i != 1 && i != 2 && i != 1) {
				std::string _response = "You must be authorized to use this command\n";
				send(_client_fd, _response.c_str(), _response.size(), 0);
				_client->_buffer_client.clear();
				return ;
			}
		}
		std::string _parsed_options = _tokens[0].substr(_tokens[0].find_first_of(" ") + 1);
		switch (i) {
			case 0:
				user(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 1:
				nick(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 2:
				join(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 3:
				privmsg(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 4:
				topic(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 5:
				_client->_buffer_client.clear();
				quit(tokenizeInput(_parsed_options), _client_fd);
				_tokens.erase(_tokens.begin());
				return ;
				break;
			case 6:
				mode(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 7:
				invite(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 8:
				kick(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 9:
				who(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 10:
				part(tokenizeInput(_parsed_options), _client_fd);
				break;
			case 11:
				if (pass(tokenizeInput(_parsed_options), _client_fd) == true)
					return ;
				break;
			default:
				sendUnknowCommand(_options, _client_fd);
				break;
		}
		_tokens.erase(_tokens.begin());
	} while (!_tokens.empty());
	_client->_buffer_client.clear();
};