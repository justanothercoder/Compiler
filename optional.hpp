#ifndef _OPTIONAL_HPP_
#define _OPTIONAL_HPP_

template <typename T>
class optional
{
public:
	optional() : engaged(false) { }
	optional(T value) : value_(value), engaged(true) { }

	optional& operator=(T value) { engaged = true; value_ = value; return *this; }
	optional& operator=(const optional& opt) { engaged = opt.engaged; if ( opt.engaged ) value_ = opt.value_; return *this; }

	T& value() { if ( engaged ) return value_; else throw "optional misuse"; }
	T value_or(T value) { return engaged ? value_ : value; }

	T& operator*() { return value(); }
	T* operator->() { if ( engaged ) return &value_; else throw "optional misuse"; }

	operator bool() { return engaged; }

	static optional empty() { return optional(); }

private:

	T value_;
	bool engaged;
};

template <typename T>
optional<T> make_optional(T value) { return optional<T>(value); }

#endif
