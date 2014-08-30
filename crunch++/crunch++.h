/*
 * This file is part of crunch
 * Copyright © 2013 Rachel Mant (dx-mon@users.sourceforge.net)
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

#ifndef __CRUNCHpp_H__
#define __CRUNCHpp_H__

#include <stddef.h>
#include <inttypes.h>
#include <thread>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <functional>

#ifdef _MSC_VER
	#ifdef __crunch_lib__
		#define CRUNCH_VIS	__declspec(dllexport)
	#else
		#define CRUNCH_VIS	__declspec(dllimport)
	#endif
	#ifdef __cplusplus
		#define CRUNCH_API	extern "C" CRUNCH_VIS
	#else
		#define CRUNCH_API	CRUNCH_VIS
	#endif
	#define CRUNCH_EXPORT		__declspec(dllexport)
#else
	#if __GNUC__ >= 4
		#define CRUNCH_VIS __attribute__ ((visibility("default")))
	#else
		#define CRUNCH_VIS
	#endif
	#ifdef __cplusplus
		#define CRUNCH_API	extern "C" CRUNCH_VIS
	#else
		#define CRUNCH_API	extern CRUNCH_VIS
	#endif
	#define CRUNCH_EXPORT		CRUNCH_API
#endif
#define CRUNCHpp_API	CRUNCH_VIS

class testsuit;

struct cxxTest
{
	std::function<void()> testFunc;
	const char *testName;
};

struct cxxTestClass
{
	testsuit *testClass;
	const char *testClassName;
};

struct cxxUnitTest
{
	std::thread testThread;
	cxxTest theTest;
};

class CRUNCHpp_API testsuit
{
protected:
	std::vector<cxxTest> tests;

	void fail(const char *reason);

	void assertTrue(bool value);
	void assertFalse(bool value);

	void assertEqual(int32_t result, int32_t expected);
	void assertEqual(int64_t result, int64_t expected);
	void assertEqual(void *result, void *expected);
	void assertEqual(double result, double expected);
	void assertEqual(const char *result, const char *expected);
	void assertEqual(const void *result, const void *expected, const size_t expectedLength);

	void assertNotEqual(int32_t result, int32_t expected);
	void assertNotEqual(int64_t result, int64_t expected);
	void assertNotEqual(void *result, void *expected);
	void assertNotEqual(double result, double expected);
	void assertNotEqual(const char *result, const char *expected);
	void assertNotEqual(const void *result, const void *expected, const size_t expectedLength);

	void assertNull(void *result);
	void assertNotNull(void *result);
	void assertNull(const void *result);
	void assertNotNull(const void *result);

	void assertGreaterThan(long result, long expected);
	void assertLessThan(long result, long expected);

	testsuit();

private:
	static int testRunner(testsuit &unitClass, cxxUnitTest &test);

public:
	virtual ~testsuit();
	virtual void registerTests() = 0;
	void test();
};

struct threadExit_t
{
private:
	int value;

public:
	threadExit_t(int exitValue) noexcept : value(exitValue) { }

	operator int() const noexcept
	{
		return value;
	}
};

CRUNCH_API std::vector<cxxTestClass> cxxTests;

template<typename TestClass> void registerTestClasses()
{
	cxxTestClass testClass = {new TestClass(), typeid(TestClass).name()};
	cxxTests.push_back(testClass);
}

template<typename TestClass, typename ...TestClasses>
typename std::enable_if<sizeof...(TestClasses), void>::type registerTestClasses()
{
	registerTestClasses<TestClass>();
	registerTestClasses<TestClasses...>();
}

#define CXX_TEST(name) \
{ \
	cxxTest test = {nullptr, #name}; \
	test.testFunc = [this](){ this->name(); }; \
	tests.push_back(test); \
}

typedef struct testLog
{
	FILE *file;
	int fd, stdout;
} testLog;

CRUNCH_API testLog *startLogging(const char *fileName);
CRUNCH_API void stopLogging(testLog *logFile);

#endif /*__CRUNCHpp_H__*/
