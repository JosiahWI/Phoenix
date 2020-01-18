// Copyright 2019 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/**
 * @file Commander.cpp
 * @brief Source file to implement a command execution system designed to
 * interface with a terminal.
 *
 * @copyright Copyright (c) 2019-2020
 *
 */

#include <Phoenix/Commander.hpp>
#include <utility>

using namespace phx;

Commander::Commander() : m_book(CommandBook::get()) {}

Commander::~Commander() {}

int CommandBook::find(const std::string& command)
{
	for (int j = 0; j < m_page; j++)
	{
		if (m_command[j] == command)
		{
			return j;
		}
	}
	return -1;
}

void CommandBook::add(const std::string& command, const std::string& help,
                      const std::string& permission, commandFunction f)
{
	int j = find(command);
	// If command does not already exist, enter new command
	if (j == -1)
	{
		j = m_page;
		m_page++;
	}
	m_command.push_back(command);
	m_help.push_back(help);
	m_permission.push_back(permission);
	m_functions.push_back(f);
}

int CommandBook::getPage() { return m_page; }

bool Commander::help(const std::vector<std::string>&& args, std::ostream& out)
{
	if (args.size() < 1)
	{
		out << "Type /help [command] to learn more about a command \nType "
		       "/list for a list of available commands\n";
		return true;
	}
	else if (args[0] == "help")
	{
		out << "Type /help [command] to learn more about a command \n";
		return true;
	}
	else if (args[0] == "list")
	{
		out << "Lists available commands\n";
		return true;
	}
	const int j = m_book->find(args[0]);
	if (j == 0)
	{
		out << "Command \"" + args[0] + "\" not found \n";
		return false;
	}
	else
	{
		out << m_book->m_help[j];
		return true;
	}
}

bool Commander::run(const std::string&               command,
                    const std::vector<std::string>&& args, std::ostream& out)
{
	// Check for built in functions
	if (command == "help")
	{
		return this->help(std::move(args), out);
	}
	else if (command == "list")
	{
		this->list(out);
		return true;
	}
	// If no built in functions match, search library
	const int j = m_book->find(command);
	if (j == -1)
	{
		out << "Command \"" + command + "\" not found \n";
		return false;
	}
	else
	{
		m_book->m_functions[j](args);
		return true;
	}
}

void Commander::list(std::ostream& out)
{
	out << "Available commands\n";
	for (int j = 0; j < m_book->getPage(); j++)
	{
		out << "-" + m_book->m_command[j] + "\n";
	}
}

void Commander::post(std::istream& in, std::ostream& out)
{
	std::string input;
	while (true)
	{
		out << "\n->";
		int                      i = 0;
		std::vector<std::string> args;
		std::string              command = "";
		std::string              buffer;
		in >> command;
		while (in.peek() != '\n')
		{
			in >> buffer;
			args.push_back(buffer);
			i++;
		}
		if (command == "exit")
		{
			break;
		}
		run(command, std::move(args), out);
	}
}

void Commander::callback(const std::string& input, std::ostringstream& cout)
{
	// easter egg commission for the tobster.
	if (input.compare("buh-buh-bum-bah-bum") == 0)
		cout << "I'll be back\n";

	cout << "->" << input << "\n";

	std::string s = input;

	if (s.substr(0, 1) == "/")
	{
		std::vector<std::string> args;

		size_t      pos     = s.find(" ");
		std::string command = s.substr(1, pos - 1);
		s.erase(0, pos + 1);
		std::string token;
		while ((pos = s.find(" ")) != std::string::npos)
		{
			token = s.substr(0, pos - 1);
			args.push_back(token);
			s.erase(0, pos + 1);
		}
		run(command, std::move(args), cout);
	}
}