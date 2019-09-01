/*
 * This file is part of crunch
 * Copyright © 2019 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * crunch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * crunch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <crunch++.h>
#include "Logger.h"
#include "Core.h"
#ifndef _WINDOWS
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include <stdio.h>

#ifndef _WINDOWS
#define DEV_NULL "/dev/null"
#else
#define DEV_NULL "NUL"
#define STDIN_FILENO fileno(stdin)
#define O_CLOEXEC O_BINARY
#endif

class loggerTests final : public testsuit
{
private:
	int nullFD = -1;
	int stdoutFD = -1;
	testLog ourLogger;

	void testColumns()
	{
		logger = &ourLogger;
		assertEqual(dup2(nullFD, STDOUT_FILENO), STDOUT_FILENO);
		assertEqual(getColumns(), 80);
		assertEqual(dup2(stdoutFD, STDOUT_FILENO), STDOUT_FILENO);
		logger = nullptr;
	}

	void testSuccess()
	{
		//logger = &pipeLogger;
		isTTY = false;
		logResult(RESULT_SUCCESS, "");
		--passes;
		isTTY = true;
		logResult(RESULT_SUCCESS, "");
		--passes;
		isTTY = isatty(STDOUT_FILENO);
		//logger = nullptr;
	}

public:
	loggerTests() noexcept : nullFD{open(DEV_NULL, O_RDONLY | O_CLOEXEC)}, stdoutFD{dup(STDOUT_FILENO)},
		ourLogger{nullptr, fdopen(stdoutFD, "w"), nullptr, 0} { }
	~loggerTests() noexcept { close(nullFD); close(stdoutFD); fclose(ourLogger.stdout_); }

	void registerTests() final override
	{
		if (nullFD == -1 || stdoutFD == -1)
			skip("Unable to open null device for tests");
		CXX_TEST(testColumns)
#ifndef _WINDOWS
		CXX_TEST(testSuccess)
#endif
	}
};

CRUNCHpp_TEST void registerCXXTests();
void registerCXXTests()
{
	registerTestClasses<loggerTests>();
}