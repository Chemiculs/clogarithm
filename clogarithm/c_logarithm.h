#ifndef CXX_LOGARITHM_H
#define CXX_LOGARITHM_H

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

namespace c_logarithm {

	typedef struct clogarithm_entry_t {

		bool valid;

		std::intptr_t base;

		std::intptr_t logarithm;
	};

	class clogarithm {

	private:

#pragma region Globals

		std::size_t logarithm_success_count_; // this is incremented when the iterator(base) is a valid logarithm of (x)

		std::size_t logarithm_failure_count_; // this is incremented when the iterator(base) fails to produce an integral logarithmic baseof(x)

		std::vector<clogarithm_entry_t> logarithms_; // details of all discovered logarithms thus far

		std::intptr_t iterator_ = 2; // current number to being used as logarithmic baseof(x), cannot be lower than 2

		volatile std::intptr_t x; //target value (x)

		volatile bool direction_; // false = backwards, true = forwards

#pragma endregion

#pragma region Helper Functions

		__declspec(noinline) void __fastcall do_tick(bool success = false) {

			if (direction_) // Self-explanatory
				++iterator_;
			else
				--iterator_;

			if (success)
				++logarithm_success_count_;
			else
				++logarithm_failure_count_;
		}

#pragma endregion

	public:

#pragma region Accessors

		inline std::size_t __cdecl get_logarithm_failure_count() {

			return logarithm_failure_count_;
		}

		inline std::size_t __cdecl get_logarithm_success_count() {
		
			return logarithm_success_count_;
		}

		inline std::intptr_t __cdecl get_x() {

			return x;
		}

		__declspec(noinline) bool __fastcall set_x(intptr_t _x) {

			if (_x == 0)
				return false;

			x = _x;

			return true;
		}

		inline void __cdecl swap_direction() {

			direction_ = direction_ ? false : true;
		}

#pragma endregion

#pragma region Core Functionality

		static __declspec(noinline) bool __fastcall is_base_logarithmic(std::intptr_t _base, std::intptr_t _x, std::intptr_t* _logarithm = nullptr) {

			std::intptr_t iteration = 0;

			std::intptr_t exponent = _base;

			while (exponent < _x) {

				exponent *= _base;
				++iteration;
			}

			if (exponent != _x)
				return false;

			if (_logarithm)
				*_logarithm = iteration;

			return true;
		}

		// Try current iterator (begins as 2) as a logarithmic base(iterator) of x and then increment or decrement the iterator, this is useful for looping / brute-forcing (return.valid = true if valid logarithmic base)
		inline const clogarithm_entry_t& __cdecl try_next_integer() {

			if (iterator_ >= x)
				return clogarithm_entry_t{ false, 0, 0 };

			std::intptr_t iteration = 0;

			const bool is_logarithmic = is_base_logarithmic(iterator_, x, &iteration);

			if (!is_logarithmic) {

				do_tick();

				return clogarithm_entry_t{ false, 0, 0 };
			}

			clogarithm_entry_t result{ true, iterator_,  std::move(iteration) + 1 };

			logarithms_.push_back(result); // Add this logarithm and it's specificities to the vector

			do_tick(true);

			return std::move(result); // Use perfect forwarding on resulting object
		}

		inline const clogarithm_entry_t& find_next_logarithmic_base() {

			clogarithm_entry_t result{ false, 0, 0 };

			while (!(result = try_next_integer()).valid)
				if (iterator_ > ((x / 2) + 1)) // 2 is the logarithmic base 2 of 4, however this is, i believe, the only instance where a logarithmic base of a number can reach (or exceed) 50% of X, so increment the point of insanity beyond 50% margin
					break;

			return std::move(result);
		}

		inline std::unique_ptr< std::vector<clogarithm_entry_t> > __cdecl find_all_logarithmic_bases() {

			if (logarithms_.size())
				logarithms_.clear();

			std::vector< clogarithm_entry_t > result{};

			std::intptr_t backup_iterator = iterator_;

			logarithm_failure_count_ = 0;

			logarithm_success_count_ = 0;

			iterator_ = 2; // reset base iterator

			clogarithm_entry_t _logarithm{ false, 0 , 0 };

			while ((_logarithm = find_next_logarithmic_base()).valid)
				result.push_back(_logarithm);

			iterator_ = std::move(backup_iterator);

			return std::make_unique<std::vector<clogarithm_entry_t>>(std::move(result));
		}

#pragma endregion

#pragma region Ctor / Dtor

		__declspec(noinline) __fastcall clogarithm(std::intptr_t _x = 0) : direction_(true) {

			try {

				set_x(_x);
			}
			catch (const std::exception& except) { throw except; } // lol
		}

		inline __cdecl ~clogarithm() {

			if (logarithms_.size())
				logarithms_.clear();
		}

#pragma endregion

	};
}

#endif