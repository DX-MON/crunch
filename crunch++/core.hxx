// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef CORE__HXX
#define CORE__HXX

#include <utility>
#include "crunch++.h"

namespace crunch
{
	struct cxxTestClass final
	{
	private:
		std::unique_ptr<testsuite> suite_{};
		const char *name_{nullptr};

	public:
		cxxTestClass() noexcept : suite_{}, name_{nullptr} { }
		cxxTestClass(std::unique_ptr<testsuite> &&suite, const char *const name) :
			suite_{std::move(suite)}, name_{name} { }
		cxxTestClass(const cxxTestClass &) = delete;
		cxxTestClass(cxxTestClass &&) = default;
		~cxxTestClass() noexcept = default;
		cxxTestClass &operator =(const cxxTestClass &) = delete;
		cxxTestClass &operator =(cxxTestClass &&) = default;

		testsuite *suite() const noexcept { return suite_.get(); }
		const char *name() const noexcept { return name_; }

		void swap(cxxTestClass &other) noexcept
		{
			auto tmp{std::move(*this)};
			*this = std::move(other);
			other = std::move(tmp);
		}
	};

	CRUNCHpp_API uint32_t passes, failures;
	CRUNCHpp_API bool loggingTests;
	CRUNCHpp_API std::vector<cxxTestClass> cxxTests;
} // namespace crunch

#endif /*CORE__HXX*/
