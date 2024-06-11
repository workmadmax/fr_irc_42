#pragma once

# include <algorithm>
# include <arpa/inet.h>
# include <csignal>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <errno.h>
# include <fcntl.h>
# include <iostream>
# include <limits>
# include <list>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>

# include "Channel.hpp"
# include "OtherMacros.hpp"
# include "Responses.hpp"
# include "Client.hpp"
# include "Server.hpp"


# define _MAX_CLIENT 777


# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define RESET "\033[0m"
# define YELLOW "\033[0;33m"