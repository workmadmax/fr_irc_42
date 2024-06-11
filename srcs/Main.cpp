/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:33:12 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 14:33:39 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

int main(int ac, char **av) // Função principal do programa, recebe argumentos de linha de comando
{ 													
	if (ac != 3)
	{ 																            // Verifica se o número de argumentos é diferente de 3
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;	// Imprime uma mensagem de uso correto do programa
		return 1; 																// Retorna 1 para indicar um erro
	}

	try
	{
		Server server; 															// Cria uma instância da classe Server
		server.setPort(av[1]); 													// Define a porta do servidor com base no primeiro argumento
		server.setPassword(av[2]); 												// Define a senha do servidor com base no segundo argumento

		signal(SIGPIPE, SIG_IGN); 												// Ignora o sinal SIGPIPE
		signal(SIGINT, server.signalHandler); 										// Define o tratamento de sinal para SIGINT usando o método handleSig da classe Server
		signal(SIGQUIT, server.signalHandler); 										// Define o tratamento de sinal para SIGQUIT usando o método handleSig da classe Server

		server.runServer(); 													// Configura o servidor
	} 
	catch (std::exception &e)
	{ 												                            // Captura exceções do tipo std::exception
		std::cerr << e.what() << std::endl; 									// Imprime a mensagem de erro da exceção
	}

	std::cout << "Server is closing..." << std::endl; 							// Imprime uma mensagem indicando que o servidor está sendo fechado
	return 0; 																	// Retorna 0 para indicar que o programa foi executado com sucesso
}