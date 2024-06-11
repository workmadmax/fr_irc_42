/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 10:24:10 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/11 19:11:11 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

static bool	checkChannelName(const std::string& _channel_name)
{
	if (_channel_name.size() < 2 || _channel_name.size() > 200) {
		return (false);
	}
	for (size_t i = 1; i < _channel_name.size(); i++) {
		if (!isalnum(_channel_name[i]) && _channel_name[i] != '+' \
			&& _channel_name[i] != '-' && _channel_name[i] != '_') {
			return (false);
		}
	}
	return (true);
};

void	splitIntoTokens(std::string& _input, char delimiter, std::vector<std::string>& _tokens)
{
    std::string	ntoken;
    std::istringstream	tokenStream(_input);
    while (std::getline(tokenStream, ntoken, delimiter)) {
        _tokens.push_back(ntoken);
        std::cout << "Token: " << ntoken << std::endl; // Imprime cada token
    }
    std::cout << "Input string after split: " << _input << std::endl;
};

static void	splitString(std::string& _input, char delimiter, std::vector<std::string>& _tokens)
{
    std::string	ntoken;
    std::istringstream	tokenStream(_input);
    while (std::getline(tokenStream, ntoken, delimiter)) {
        _tokens.push_back(ntoken);
        std::cout << "Token: " << ntoken << std::endl; // Imprime cada token
    }
    std::cout << "funciono: " << _input << std::endl;
};

void Server::join(std::vector<std::string> _input_cmd, int _client_fd)
{
    bool _is_valid = false;

    std::string 				_response;
    std::vector<std::string> 	_channels;

    if (_input_cmd[0].empty() || (_input_cmd.size() > 1 && _input_cmd[1].empty()))
    {
        _response = "JOIN :Not enough parameters" + END;
        send(_client_fd, _response.c_str(), _response.size(), 0);
        return;
    }
	std::cout << RED << "passou aqui" << RESET << std::endl;

    splitString(_input_cmd[0], ',', _channels);
    std::cout << "Number of channels: " << _channels.size() << std::endl; // Imprime o número de canais

    std::vector<std::string> _passwords;
    if (_input_cmd.size() > 1)
    	splitString(_input_cmd[1], ',', _passwords);

	
    Client* _client = handleClientFd(_client_fd);
    for (size_t i = 0; i < _channels.size(); i++)
    {
        std::cout << "Processing channel: " << _channels[i] << std::endl; // Imprime o canal atual

        if (!checkChannelName(_channels[i]))
        {
            _response = "JOIN :Invalid channel name" + _channels[i] + END;
            send(_client_fd, _response.c_str(), _response.size(), 0);
            continue ;
        }

        if (!channelExists(_channels[i]))
        {
            createChannel(_channels[i]);
            _is_valid = true;
        }

        Channel *channelPtr = getChannel(_channels[i]);
        if (channelPtr->getMode("i") == true)
        {
            if (!channelPtr->checkInvite(_client->getNick()))
            {
                _response = "JOIN :" + _channels[i] + " " + _channels[i] + " ERR_INVITEONLYCHAN" + END;
                send(_client_fd, _response.c_str(), _response.size(), 0);
                continue ;
            }
            else
                channelPtr->removeInvList(_client->getNick());
        }

        std::string password = channelPtr->getPassword();
        if (!password.empty() && _passwords.size() < i + 1)
        {
            _response = "JOIN :" + _channels[i] + " " + _channels[i] + " ERR_PASSWDMISMATCH" + END;
            send(_client_fd, _response.c_str(), _response.size(), 0);
            continue ;
        }
        if (!password.empty() && password != _passwords[i])
        {
            _response = "JOIN :" + _channels[i] + " " + _channels[i] + " ERR_PASSWDMISMATCH" + END;
            send(_client_fd, _response.c_str(), _response.size(), 0);
            continue ;
        }

        if (channelPtr->getUserCount() < channelPtr->getUserLimit())
        {
            channelPtr->addClient(_client);
        }
        else
        {
            _response = "JOIN :" + _channels[i] + " " + _channels[i] + " ERR_CHANNELISFULL" + END;
            send(_client_fd, _response.c_str(), _response.size(), 0);
            continue ;
        }
        if (_is_valid)
        {
            channelPtr->promoteClient(_client->getNick());
        }

        std::cout << " Joining channel: " << _channels[i] << std::endl;
        std::string _response = ":" + _client->getNick() + " JOIN " + _channels[i] + END;
        send(_client_fd, _response.c_str(), _response.size(), 0);

        if (!channelPtr->getTopic().empty())
        {
            _response = "TOPIC " + _channels[i] + " :" + channelPtr->getTopic() + END;
            send(_client_fd, _response.c_str(), _response.size(), 0);
        }
    }
};