#pragma once

# include "Libs.hpp"

class Client;
class Channel;

class Server{
	private:
		int							_port; // Número da porta para o servidor
		int							_socket_fd; // Descritor de arquivo do socket
		static bool					_signals; // Flag para lidar com sinais
		std::string					_password; // Senha para acesso ao servidor
		std::vector<Channel>		_channels; // Vetor de canais
		std::vector<Client*>		_Clients; // Vetor de usuários
		std::vector<struct pollfd>	_poll_fds; // Vetor de descritores de arquivo
		// o nome do servidor
		std::string					_server_name;


	public:
		// Setters
		void						setPort(char *_input);
		void						setPassword(char *_input);
		
	
		// Getters
		int							getPort();
		std::string					getPassword();
		bool						channelExists(const std::string& _channel_name) const;
		Channel*					getChannel(const std::string& _channel_name);
		Client*						handleClientFd(int _client_fd);
		Client*						findClientByNick(const std::string _nickname);
		std::vector<Client*>		getClients();
	

		// methods functions
		void						listenClient(int _client_fd);
		void						handleNewConnection(void);
		void						removeClient(int _client_fd);
		void						closeClientFd();
		void						freeClients();
		static void					signalHandler(int _signal);
		void						initializeSocket();
		void						runServer();
		void						createChannel(const std::string& _channel_name);
		void						removeChannel(const std::string& _channel_name);
		// parse commands
		std::vector<std::string>	tokenizeInput(std::string _command);
		std::vector<std::string> 	tokenize_buffer(std::string _buffer);
		void						sendUnknowCommand(std::string _command, int _client_fd);
		void						setupOptions(std::string& _buffer, int _client_fd);

		// commands
		void						join(std::vector<std::string> _cmd_args, int _client_fd);
		void						who(std::vector<std::string> _cmd_args, int _client_fd);
		void						topic(std::vector<std::string> _cmd_args, int _client_fd);
		void						quit(std::vector<std::string> _cmd_args, int _client_fd);
		void						nick(std::vector<std::string> _cmd_args, int _client_fd);
		void						invite(std::vector<std::string> _cmd_args, int _client_fd);
		void						kick(std::vector<std::string> _cmd_args, int _client_fd);
		void						part(std::vector<std::string> _cmd_args, int _client_fd);
		void						privmsg(std::vector<std::string> _cmd_args, int _client_fd);
		void						user(std::vector<std::string> _cmd_args, int _client_fd);
		void						mode(std::vector<std::string> _cmd_args, int _client_fd);
		bool						pass(std::vector<std::string> _cmd_args, int _client_fd);
		void						cap(int _client_fd); // precisa para conseguir se autorizar lul
};
