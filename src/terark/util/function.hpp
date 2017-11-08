/* vim: set tabstop=4 : */
#pragma once

#ifdef TERARK_FUNCTION_USE_BOOST
	#include <boost/function.hpp>
#else
	#include <functional>
#endif

namespace terark {

#ifdef TERARK_FUNCTION_USE_BOOST
	using boost::function;
	using boost::ref;
	using boost::cref;
	using boost::reference_wrapper
#else
	using std::function;
	using std::ref;
	using std::cref;
	using std::reference_wrapper;
#endif

	template<class FuncProto>
	class tfunc : public function<FuncProto> {
		typedef function<FuncProto> super;
	public:
		using super::super;
		template<class Functor>
		tfunc(const Functor* f) : super(ref(*f)) {}
	};

	template<class Func>
	class OnScopeExit {
		const Func& on_exit;
	public:
		OnScopeExit(const Func& f) : on_exit(f) {}
		~OnScopeExit() { on_exit(); }
	};
#define TERARK_SCOPE_EXIT(...) \
    auto BOOST_PP_CAT(func_on_exit_,__LINE__) = [&]() { __VA_ARGS__; }; \
    terark::OnScopeExit<decltype(BOOST_PP_CAT(func_on_exit_,__LINE__))> \
        BOOST_PP_CAT(call_on_exit_,__LINE__)(BOOST_PP_CAT(func_on_exit_,__LINE__))


} // namespace terark
