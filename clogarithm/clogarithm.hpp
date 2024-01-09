/*
/********************************************************************
*                                                                  *
*     Highly optimized Logarithmic base finder written in C++      *
*				This object uses C++ exceptions					   *
*						© Lane Graham, 2024                        *
*			https://github.com/Chemiculs/clogarithm                *
*                                                                  *
*********************************************************************
*/

#pragma region Header Guard

#ifndef CXX_LOGARITHM_H
#define CXX_LOGARITHM_H

#pragma endregion

#pragma region Constants

//i have encountered undefined behavior which i currently haven't had the time to debug when this engine targets the below resulting numbers as X, so we are excluding them from the class at the moment to prevent an infinite loop
// we are excluding the outlier / exceptional cases of 0, 1, and -1 as being valid logarithms.
// 0*0 = 0
// 1*1 = 1
// -1*1 = -1


// X_MIN values are simply added for aesthetic - any number +2 or higher, or -2 and lower will succeed as (X)

#define CLOGARITHM_BASE_B_MIN_POSITIVE 2i8   // minimum positive number capable of fulfilling (b) in a logarithmic function 
#define CLOGARITHM_X_MIN_POSITIVE 4i8    // minimum positive target (x) bearing a logarithmic base which is not itself
#define CLOGARITHM_BASE_B_MIN_NEGATIVE -2i8  // minimum negative number capable of fulfilling (b) a logarithmic function
#define CLOGARITHM_X_MIN_NEGATIVE -4i8       // minimum negative target (x) bearing a logarithmic base which is not itself

#pragma endregion

#pragma region Imports

#include <exception>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

#pragma endregion

namespace clogarithm {

#pragma region Data Structures

	/* base structure used to describe a logarithmic expression of(x) */

	typedef struct clogarithm_entry_t {

		bool valid; // is this a trash return value or is this a valid logarithmic expression of(x) ?

		std::intptr_t base; // output expression base

		std::intptr_t logarithm; // output exponent exponent
	};

#pragma endregion

	/* Base algorithmic object */

	class clogarithm_t {

	private:

#pragma region Globals

		std::size_t logarithm_success_count_; // this is incremented when the iterator(base) is a valid logarithm of (x)

		std::size_t logarithm_failure_count_; // this is incremented when the iterator(base) fails to produce an integral logarithmic baseof(x)

		std::vector<clogarithm_entry_t> logarithms_; // details of all discovered logarithms thus far

		std::intptr_t iterator_ = 2; // current number to being used as logarithmic baseof(x), cannot be lower than 2

		std::intptr_t x; // target value (x)

		bool direction_; // false = backwards, true = forwards

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

		inline void __cdecl clear_logarithm_engine() {

			if (logarithms_.size())
				logarithms_.clear();

			logarithm_failure_count_ = 0;

			logarithm_success_count_ = 0;

			iterator_ = CLOGARITHM_BASE_B_MIN_POSITIVE; // reset base iterator
		}

		inline std::size_t __cdecl get_logarithm_failure_count() { // failed logarithmic baseof(iterator) found thus far

			return logarithm_failure_count_;
		}

		inline std::size_t __cdecl get_logarithm_success_count() { // succesful logarithmic baseof(iterator) found thus far

			return logarithm_success_count_;
		}

		inline std::intptr_t __cdecl get_x() { // return target result 

			return x;
		}

		__declspec(noinline) bool __fastcall set_x(intptr_t _x) { // set target expression result

			clear_logarithm_engine();

			if (!((_x <= CLOGARITHM_BASE_B_MIN_NEGATIVE) || (_x >= CLOGARITHM_BASE_B_MIN_POSITIVE)))
				throw new std::exception("Invalid target(x) specified in logorithm!");
				

			x = _x;

			return true;
		}

		inline void __cdecl swap_direction() { // true = search forward, false = search backwards

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

			if (_logarithm != 0)
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

				do_tick(false);

				return clogarithm_entry_t{ false, 0, 0 };
			}

			clogarithm_entry_t result{ true, iterator_,  std::move(iteration) + 1 };

			logarithms_.push_back(result); // Add this logarithmic expression to the vector

			do_tick(true);

			return std::move(result); // reference this object as in our return
		}

		inline const clogarithm_entry_t find_next_logarithmic_base() {

			bool hit = false;

			clogarithm_entry_t result{ false, 0, 0 };

			while (true) {

				result = try_next_integer();

				if (result.valid) {
					hit = true;
					break;
				}

				if (iterator_ > (x / 2)) // 2 is the logarithmic base 2 of 4, however this is, i believe, the only instance where a logarithmic base of a number can reach (or exceed) 50% of X, so increment the point of insanity beyond 50% margin
					break;                  // tl;dr sanity check
			}

			return hit ? std::move(logarithms_[logarithms_.size() - 1]) : clogarithm_entry_t{ false, 0 , 0 }; // return last entry in the array if successful as this describes the successful logarithmic expression
		}

		inline std::unique_ptr< std::vector<clogarithm_entry_t> > __cdecl find_all_logarithmic_bases() {

			clear_logarithm_engine();

			while (find_next_logarithmic_base().valid)
				continue;

			auto _return = logarithms_;

			return std::make_unique<std::vector<clogarithm_entry_t>>(_return);
		}

#pragma endregion

#pragma region Ctor / Dtor

		__declspec(noinline) __fastcall clogarithm_t(std::intptr_t _x = 2) : direction_(true) {

			try {

				set_x(_x);
			}
			catch (const std::exception& except) { throw except; } // exception should never be thrown here
		}

		inline __cdecl ~clogarithm_t() {

			try {

				if (logarithms_.size())
					clear_logarithm_engine();
			}
			catch (const std::exception& except) { throw except; } // exception should never be thrown here
		}

#pragma endregion

	};
}

#endif
