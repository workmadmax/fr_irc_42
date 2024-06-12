/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:12:51 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 17:50:35 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/**
 * @brief Listens for incoming data from a client and handles it.
 *
 * This function is responsible for receiving data from a client socket and handling it accordingly.
 * It receives the client socket file descriptor as a parameter and creates a new Client object using the handleClientFd() function.
 * It then receives data from the client using the recv() function and stores it in a buffer.
 * If an error occurs during the receive operation, the function clears the client's buffer, removes the client from the server, closes the client socket, and returns.
 * Otherwise, it appends the received data to the client's buffer.
 * If the buffer does not contain a newline character, the function returns without further processing.
 * If the buffer contains a newline character, the function calls the setupOptions() function to process the received data and set up the appropriate options for the client.
 *
 * @param _client_fd The file descriptor of the client socket.
 */
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

/**
 * @brief Sets the port number for the server.
 * 
 * This function takes a string representation of the port number as input and converts it to an integer.
 * It then checks if the port number is within the valid range of 1024 to 49151.
 * If the port number is not within the valid range, it throws a std::runtime_error and exits the program.
 * 
 * @param _input The string representation of the port number.
 */
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

/**
 * @brief Sets the password for the server.
 * 
 * @param _input The password to be set.
 */
void	Server::setPassword(char *_input)
{
	_password = _input;
};

// getters

int		Server::getPort()
{
	return (_port);
};


/**
 * @brief Get the password of the server.
 * 
 * @return The password of the server.
 */
std::string	Server::getPassword()
{
	return (_password);
};

/**
 * @brief Checks if a channel with the given name exists.
 * 
 * @param _channel_name The name of the channel to check.
 * @return true if the channel exists, false otherwise.
 */
bool	Server::channelExists(const std::string& _channel_name) const
{
	for (std::vector<Channel>::const_iterator i = _channels.begin(); i != _channels.end(); i++) {
		if (i->getName() == _channel_name)
			return (true);
	}
	return (false);
};

/**
 * @brief Retrieves a channel object by its name.
 * 
 * This function searches for a channel in the vector of channels based on its name.
 * If a channel with the specified name is found, a pointer to the channel object is returned.
 * If no channel is found, a null pointer is returned.
 * 
 * @param _channel_name The name of the channel to retrieve.
 * @return A pointer to the channel object if found, otherwise a null pointer.
 */
Channel*	Server::getChannel(const std::string& _channel_name)
{
	for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
		if (i->getName() == _channel_name)
			return (&(*i));
	}
	return (NULL);
};

/**
 * @brief Finds and returns the Client object associated with the given client file descriptor.
 * 
 * This function iterates through the list of clients and checks if the client file descriptor matches the given one.
 * If a match is found, the corresponding Client object is returned.
 * 
 * @param _client_fd The client file descriptor to search for.
 * @return A pointer to the Client object if found, otherwise NULL.
 */
Client*	Server::handleClientFd(int _client_fd)
{
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		if (_Clients[i]->getClientFd() == _client_fd)
			return (_Clients[i]);
	}
	return (NULL);
};

/**
 * @brief Finds a client by nickname.
 * 
 * This function searches for a client in the list of clients based on their nickname.
 * 
 * @param _nickname The nickname of the client to find.
 * @return A pointer to the found client, or NULL if no client with the given nickname is found.
 */
Client*	Server::findClientByNick(const std::string _nickname)
{
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		if (_Clients[i]->getNick() == _nickname)
			return (_Clients[i]);
	}
	return (NULL);
};

/**
 * @brief Get the clients connected to the server.
 * 
 * @return std::vector<Client*> The vector of clients.
 */
std::vector<Client*>	Server::getClients()
{
	return (_Clients);
};

// methods functions
/**
 * @brief Handles a new incoming connection.
 *
 * This function is responsible for accepting a new client connection, setting it to non-blocking mode,
 * and adding it to the list of connected clients and poll file descriptors.
 *
 * @throws std::runtime_error if accept() or fcntl() fails.
 */
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

/**
 * @brief Removes a client from the server.
 *
 * This function removes a client from the server by removing its file descriptor from the poll file descriptors vector
 * and erasing the client object from the clients vector.
 *
 * @param _client_fd The file descriptor of the client to be removed.
 */
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

/**
 * @brief Closes the file descriptors of all connected clients and the server socket.
 */
void	Server::closeClientFd()
{
	for (size_t i = 0; i < _Clients.size(); i++) {
		close (_Clients[i]->getClientFd());
	}
	if (_socket_fd != -1)
		close(_socket_fd);
}; 

/**
 * @brief Frees the memory allocated for all clients in the server.
 *
 * This function iterates over the `_Clients` vector and deletes each client object.
 * It is important to call this function before destroying the server object to avoid memory leaks.
 */
void	Server::freeClients()
{
	for (size_t i = 0; i < _Clients.size(); i++) {
		delete _Clients[i];
	}
};

bool	Server::_signals = true;
/**
 * @brief Handles the signal received by the server.
 *
 * This function is called when the server receives a SIGINT or SIGTERM signal.
 * It sets the `_signals` flag to false, indicating that the server should stop.
 *
 * @param _signal The signal received by the server.
 */
void	Server::signalHandler(int _signal)
{
	if (_signal == SIGINT || _signal == SIGTERM)
		_signals = false;
};

/**
 * @brief Initializes the socket for the server.
 *
 * This function creates a socket, sets socket options, binds the socket to a specific address and port,
 * and starts listening for incoming connections. It also adds the socket file descriptor to the poll
 * file descriptor list for event monitoring.
 */
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

/**
 * @brief Runs the server and handles incoming connections.
 * 
 * This function initializes the socket and enters a loop to continuously
 * handle incoming connections. It uses the poll() function to wait for
 * events on the file descriptors associated with the server socket and
 * client sockets. When a new connection is detected, the function calls
 * the handleNewConnection() function. For existing connections, it calls
 * the listenClient() function to process incoming data from the clients.
 * 
 * @throws std::runtime_error if the poll() function fails.
 */
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

/**
 * @brief Creates a new channel with the given name and adds it to the server's list of channels.
 * 
 * @param _channel_name The name of the channel to create.
 */
void	Server::createChannel(const std::string& _channel_name)
{
	Channel new_channel(_channel_name);
	_channels.push_back(new_channel);
};

/**
 * @brief Removes a channel from the server.
 * 
 * This function removes the channel with the specified name from the server. If the channel name is empty, the function returns without doing anything.
 * 
 * @param _channel_name The name of the channel to be removed.
 */
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


