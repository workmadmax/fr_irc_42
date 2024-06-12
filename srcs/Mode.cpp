/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouglas <mdouglas@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:39:47 by mdouglas          #+#    #+#             */
/*   Updated: 2024/06/12 18:35:41 by mdouglas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Libs.hpp"

namespace mode
{
	void setOperatorMode(Channel *_channel, std::string _mode)
	{
		_channel->promoteClient(_mode);
	};
	void unsetOperatorMode(Channel *_channel, std::string _mode)
	{
		_channel->downGradeClient(_mode);
	};

	void setTopicMode(Channel *_channel)
	{
		_channel->setMode("t", true);
	};
	void unsetTopicMode(Channel *_channel)
	{
		_channel->setMode("t", false);
	};

	void setInviteMode(Channel *_channel)
	{
		_channel->setMode("i", true);
	};
	void unsetInviteMode(Channel *_channel)
	{
		_channel->setMode("i", false);
	};

	void setPasswordMode(Channel *_channel, std::string _pass)
	{
		_channel->setMode("k", true);
		_channel->setPassword(_pass);
	};
	void unsetPasswordMode(Channel *_channel)
	{
		std::string _pass = "";
		_channel->setMode("k", false);
		_channel->setPassword(_pass);
	};

	void setLimitMode(Channel *_channel, std::string _param)
	{
		int _limit;
		std::stringstream _ss(_param);

		_channel->setMode("l", true);
		_ss >> _limit;
		_channel->setMaxClient(_limit);
	};
	void unsetLimitMode(Channel *_channel)
	{
		_channel->setMode("l", false);
		_channel->setMaxClient(_MAX_CLIENT);
	};
};

void Server::mode(std::vector<std::string> _cmd_args, int _client_fd)
{
	std::string _mode;
	std::string _response;
	std::string _channel_name = _cmd_args[0];

	Client *_client = handleClientFd(_client_fd);
	Channel *_channel_ptr = getChannel(_channel_name);

	if (_channel_ptr == NULL)
	{
		_response = ":" + IRC + " 403 " + _client->getNick() + " " + _channel_name + " :No such channel" + END;
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return;
	}
	if (_cmd_args.size() == 1 || _cmd_args[1] == "")
	{
		std::string _modes = _channel_ptr->getAllModes();
		_response = "Channel " + _channel_name + " modes: " + _modes;
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return;
	}
	if (!_channel_ptr->checkOperator(_client->getNick()))
	{
		_response = ":" + IRC + " 482 " + _client->getNick() + " " + _channel_name + " :You're not channel operator" + END;
		send(_client_fd, _response.c_str(), _response.size(), 0);
		return;
	}

	int i = 0;
	std::string _modes[] = {"-i", "+i", "-k", "+k", "-l", "+l", "-t", "+t", "-o", "+o"};
	_mode = _cmd_args[1].substr(0, _cmd_args[1].find('\r'));
	std::string _param = "";

	if (_cmd_args.size() > 2)
		_param = _cmd_args[2];
	Client *_client_ptr = findClientByNick(_param);
	_response = ":" + IRC + _client->getNick() + " MODE " + _channel_name + " " + _mode + " " + _param + END;
	for (; i < 10; i++)
	{
		if (_mode == _modes[i])
			break;
	}

	switch (i)
	{
	case 0:
		mode::unsetInviteMode(_channel_ptr);
		_response += "-i";
		break;

	case 1:
		mode::setInviteMode(_channel_ptr);
		_response += "+i";
		break;

	case 2:
		mode::unsetTopicMode(_channel_ptr);
		_response += "-t";
		break;

	case 3:
		mode::setTopicMode(_channel_ptr);
		_response += "+t";
		break;

	case 4:
		mode::unsetPasswordMode(_channel_ptr);
		_response += "-k";
		break;

	case 5:
		if (_param == "")
		{
			_response = IRC + _param + _client->getNick() + _channel_name + " " + _channel_name + " k * :You must specify a parameter for the key mode. Syntax: <key>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		mode::setPasswordMode(_channel_ptr, _param);
		_response += "+k";
		break;

	case 6:
		if (_param == "")
		{
			_response = IRC + _param + _client->getNick() + _channel_name + " " + _channel_name + " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		if (_client_ptr == NULL)
		{
			_response = IRC + _param + " :" + _channel_name + " " + _channel_name + " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		if (!_channel_ptr->checkOnChannel(_client_ptr->getNick()) || !_channel_ptr->checkOperator(_client_ptr->getNick()))
			return;

		mode::unsetOperatorMode(_channel_ptr, _param);
		_response += "-o";
		break;

	case 7:
		if (_param == "")
		{
			_response = IRC + _param + " :" + _channel_name + " " + _channel_name + " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		if (_client_ptr == NULL)
		{
			_response = IRC + _param + " :" + _channel_name + " " + _channel_name + " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		if (!_channel_ptr->checkOnChannel(_client_ptr->getNick()) || _channel_ptr->checkOperator(_client_ptr->getNick()))
		{
			return;
		}

		mode::setOperatorMode(_channel_ptr, _param);
		_response += "+o";
		break;

	case 8:
		mode::unsetLimitMode(_channel_ptr);
		_response += "-l";
		break;

	case 9:
		if (_param == "")
		{
			_response = IRC + _client->getNick() + _channel_name + " " + _channel_name + \
				" l * :You must specify a parameter for the limit mode. Syntax: <limit>." + END;
			send(_client_fd, _response.c_str(), _response.size(), 0);
			return;
		}

		mode::setLimitMode(_channel_ptr, _param);
		_response += "+l";
		break;

	default:
		_response = "Invalid mode";
		break;
	}
}