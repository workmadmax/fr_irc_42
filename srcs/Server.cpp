/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:12:51 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 18:47:16 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

// construtor
void	Server::listenClient(int _client_fd)
{
	Client*	new_client = handleClientFd(_client_fd);
	char	buffer[1024];
	memset(buffer, 0, 1024);

	ssize_t bytes_received = recv(_client_fd, buffer, 1024, 0);
	if (bytes_received < 0) {
		new_client->_buffer_client.clear();
		Server::removeClient(_client_fd);
		close(_client_fd);
		return ;
	}
	new_client->_buffer_client.append(buffer);
	if (new_client->_buffer_client.find("\n") == std::string::npos)
		return ;
	Server::setupOptions(new_client->_buffer_client, _client_fd);
};

// setters

void	Server::setPort(char *_input)
{
	std::stringstream ss(_input);
	int port;
	ss >> port;
	if (port < 1024 || port > 49151) {
		throw std::runtime_error("Port must be between 1024 and 49151");
		exit(EXIT_FAILURE);
	}
	_port = port;
};

void	Server::setPassword(char *_input)
{
	_password = _input;
};

// getters

int		Server::getPort()
{
	return (_port);
};


std::string	Server::getPassword()
{
	return (_password);
};

bool	Server::channelExists(const std::string& _channel_name) const
{
	for (std::vector<Channel>::const_iterator i = _channels.begin(); i != _channels.end(); i++) {
		if (i->getName() == _channel_name)
			return (true);
	}
	return (false);
};

Channel*	Server::getChannel(const std::string& _channel_name)
{
	for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
		if (i->getName() == _channel_name)
			return (&(*i));
	}
	return (NULL);
};

Client*	Server::handleClientFd(int _client_fd)
{
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		if (_Clients[i]->getClientFd() == _client_fd)
			return (_Clients[i]);
	}
	return (NULL);
};

Client*	Server::findClientByNick(const std::string _nickname)
{
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		if (_Clients[i]->getNick() == _nickname)
			return (_Clients[i]);
	}
	return (NULL);
};

std::vector<Client*>	Server::getClients()
{
	return (_Clients);
};

// methods functions
void	Server::handleNewConnection(void)
{
	int					new_socket_fd;
	struct sockaddr_in	client_addr;
	struct pollfd		poll_fd;
	socklen_t			client_len;
	
	client_len = sizeof(client_addr);
	new_socket_fd = accept(_socket_fd, (struct sockaddr*)&client_addr, &client_len);

	if (new_socket_fd < 0)
		throw std::runtime_error("accept() failed");
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");

	poll_fd.fd = new_socket_fd;
	poll_fd.events = POLLIN;
	poll_fd.revents = 0;
	
	Client* client = new Client(new_socket_fd);
	client->setIP(inet_ntoa(client_addr.sin_addr));
	_Clients.push_back(client);
	_poll_fds.push_back(poll_fd);
};

void	Server::removeClient(int _client_fd)
{
	for (size_t i = 0; i < _poll_fds.size(); i++) {
		if (_poll_fds[i].fd == _client_fd) {
			_poll_fds.erase(_poll_fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < _Clients.size(); i++) {
		if (_Clients[i]->getClientFd() == _client_fd) {
			_Clients.erase(_Clients.begin() + i);
			break;
		} 
	}
};

void	Server::closeClientFd()
{
	for (size_t i = 0; i < _Clients.size(); i++) {
		close (_Clients[i]->getClientFd());
	}
	if (_socket_fd != -1)
		close(_socket_fd);
}; 

void	Server::freeClients()
{
	for (size_t i = 0; i < _Clients.size(); i++) {
		delete _Clients[i];
	}
};

bool	Server::_signals = true;
void	Server::signalHandler(int _signal)
{
	if (_signal == SIGINT || _signal == SIGTERM)
		_signals = false;
};

void	Server::initializeSocket()
{
	int					opt = 1;
	struct sockaddr_in	server_addr;
	struct pollfd		new_poll_fd;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	// imprima o socket fd para depurar
	std::cout << "Socket fd: " << _socket_fd << std::endl;
	if (_socket_fd < 0)
		throw std::runtime_error("socket() failed");
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt() failed");
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");
	if (bind(_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		throw std::runtime_error("bind() failed");
	if (listen(_socket_fd, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed");
	
	new_poll_fd.fd = _socket_fd;
	new_poll_fd.events = POLLIN;
	new_poll_fd.revents = 0;
	_poll_fds.push_back(new_poll_fd);
};

void	Server::runServer()
{
	this->initializeSocket();

	while (_signals) {
		if (poll(&_poll_fds[0], _poll_fds.size(), -1) == -1 && _signals)
			throw std::runtime_error("poll() failed");
		for (size_t i = 0; i < _poll_fds.size(); i++) {
			if (_poll_fds[i].revents & POLLIN) {
				if (_poll_fds[i].fd == _socket_fd)
					handleNewConnection();
				else
					listenClient(_poll_fds[i].fd);
			}
		}
	}	
};

void	Server::createChannel(const std::string& _channel_name)
{
	Channel new_channel(_channel_name);
	_channels.push_back(new_channel);
};

void	Server::removeChannel(const std::string& _channel_name)
{
	if (_channel_name.empty())
		return;
	for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
		if (i->getName() == _channel_name) {
			_channels.erase(i);
			break;
		}
	}
};


