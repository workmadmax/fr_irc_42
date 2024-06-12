/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:24:46 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 19:00:20 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Libs.hpp"

class Client
{
	private:
			int							_client_fd; // Descritor de arquivo do cliente
			std::string					_client_IP; // IP do cliente
			std::string					_nickname; // Nick do cliente
			std::string					_username; // Username do cliente
			std::string					_hostname; // Nome real do cliente
			std::string					_password; // Senha do cliente
	
	public:
			Client(const int _client_fd); // Construtor
			std::string 				_buffer_client; // Buffer do cliente
			int							getClientFd(); // Retorna o descritor de arquivo do cliente
			bool						isAuthorized(); // Verifica se o cliente está autorizado
			std::string					getNick(); // Retorna o nick do cliente
			std::string					getUsername(); // Retorna o username do cliente
			std::string					getHostname(); // Retorna o nome real do cliente
			std::string					getPassword(); // Retorna a senha do cliente
			std::string					getIP(); // Retorna o IP do cliente
			
			void						setNick(std::string nick); // Define o nick do cliente
			void						setUsername(std::string username); // Define o username do cliente
			void						setHostname(std::string hostname); // Define o nome real do cliente
			void						setPassword(std::string password); // Define a senha do cliente
			void						setIP(std::string ip); // Define o IP do cliente
};