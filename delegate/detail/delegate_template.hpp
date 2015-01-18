/*
	(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

	Template file. May be included many times with different predefined macros.
*/
#if COHEAR_DELEGATE_PARAM_COUNT > 0
#define COHEAR_DELEGATE_SEPARATOR ,
#else
#define COHEAR_DELEGATE_SEPARATOR
#endif

// see BOOST_JOIN for explanation
#define COHEAR_DELEGATE_JOIN_MACRO( X, Y) COHEAR_DELEGATE_DO_JOIN( X, Y )
#define COHEAR_DELEGATE_DO_JOIN( X, Y ) COHEAR_DELEGATE_DO_JOIN2(X,Y)
#define COHEAR_DELEGATE_DO_JOIN2( X, Y ) X##Y

namespace chr {
namespace detail {

#ifdef COHEAR_DELEGATE_PREFERRED_SYNTAX
#define COHEAR_DELEGATE_CLASS_NAME delegate
#define COHEAR_DELEGATE_INVOKER_CLASS_NAME delegate_invoker
#else
#define COHEAR_DELEGATE_CLASS_NAME COHEAR_DELEGATE_JOIN_MACRO(delegate,COHEAR_DELEGATE_PARAM_COUNT)
#define COHEAR_DELEGATE_INVOKER_CLASS_NAME COHEAR_DELEGATE_JOIN_MACRO(delegate_invoker,COHEAR_DELEGATE_PARAM_COUNT)
	template <typename R COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_TEMPLATE_PARAMS>
	class COHEAR_DELEGATE_INVOKER_CLASS_NAME;
#endif

	template <typename R COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_TEMPLATE_PARAMS>
#ifdef COHEAR_DELEGATE_PREFERRED_SYNTAX
	class COHEAR_DELEGATE_CLASS_NAME<R (COHEAR_DELEGATE_TEMPLATE_ARGS)>
#else
	class COHEAR_DELEGATE_CLASS_NAME
#endif
	{
	public:
		typedef R return_type;
#ifdef COHEAR_DELEGATE_PREFERRED_SYNTAX
		typedef return_type (COHEAR_DELEGATE_CALLTYPE *signature_type)(COHEAR_DELEGATE_TEMPLATE_ARGS);
		typedef COHEAR_DELEGATE_INVOKER_CLASS_NAME<signature_type> invoker_type;
#else
		typedef COHEAR_DELEGATE_INVOKER_CLASS_NAME<R COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_TEMPLATE_ARGS> invoker_type;
#endif

		COHEAR_DELEGATE_CLASS_NAME()
			: object_ptr(0)
			, stub_ptr(0)
		{}

		template <return_type (*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS)>
		static COHEAR_DELEGATE_CLASS_NAME from_function()
		{
			return from_stub(0, &function_stub<TMethod>);
		}

		template <class T, return_type (T::*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS)>
		static COHEAR_DELEGATE_CLASS_NAME from_method(T* object_ptr)
		{
			return from_stub(object_ptr, &method_stub<T, TMethod>);
		}

		template <class T, return_type (T::*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS) const>
		static COHEAR_DELEGATE_CLASS_NAME from_const_method(T const* object_ptr)
		{
			return from_stub(const_cast<T*>(object_ptr), &const_method_stub<T, TMethod>);
		}

		return_type operator()(COHEAR_DELEGATE_PARAMS) const
		{
			return (*stub_ptr)(object_ptr COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_ARGS);
		}

		operator bool () const
		{
			return stub_ptr != 0;
		}

		bool operator!() const
		{
			return !(operator bool());
		}

	private:
		
		typedef return_type (COHEAR_DELEGATE_CALLTYPE *stub_type)(void* object_ptr COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_PARAMS);

		void* object_ptr;
		stub_type stub_ptr;

		static COHEAR_DELEGATE_CLASS_NAME from_stub(void* object_ptr, stub_type stub_ptr)
		{
			COHEAR_DELEGATE_CLASS_NAME d;
			d.object_ptr = object_ptr;
			d.stub_ptr = stub_ptr;
			return d;
		}

		template <return_type (*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS)>
		static return_type COHEAR_DELEGATE_CALLTYPE function_stub(void* COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_PARAMS)
		{
			return (TMethod)(COHEAR_DELEGATE_ARGS);
		}

		template <class T, return_type (T::*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS)>
		static return_type COHEAR_DELEGATE_CALLTYPE method_stub(void* object_ptr COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_PARAMS)
		{
			T* p = static_cast<T*>(object_ptr);
			return (p->*TMethod)(COHEAR_DELEGATE_ARGS);
		}

		template <class T, return_type (T::*TMethod)(COHEAR_DELEGATE_TEMPLATE_ARGS) const>
		static return_type COHEAR_DELEGATE_CALLTYPE const_method_stub(void* object_ptr COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_PARAMS)
		{
			T const* p = static_cast<T*>(object_ptr);
			return (p->*TMethod)(COHEAR_DELEGATE_ARGS);
		}
	};

	template <typename R COHEAR_DELEGATE_SEPARATOR COHEAR_DELEGATE_TEMPLATE_PARAMS>
#ifdef COHEAR_DELEGATE_PREFERRED_SYNTAX
	class COHEAR_DELEGATE_INVOKER_CLASS_NAME<R (COHEAR_DELEGATE_TEMPLATE_ARGS)>
#else
	class COHEAR_DELEGATE_INVOKER_CLASS_NAME
#endif
	{
		COHEAR_DELEGATE_INVOKER_DATA

	public:
		COHEAR_DELEGATE_INVOKER_CLASS_NAME(COHEAR_DELEGATE_PARAMS)
#if COHEAR_DELEGATE_PARAM_COUNT > 0
			:
#endif
			COHEAR_DELEGATE_INVOKER_INITIALIZATION_LIST
		{
		}

		template <class TDelegate>
		R operator()(TDelegate d) const
		{
			return d(COHEAR_DELEGATE_ARGS);
		}
	};
} // namespace detail
} // namespace chr

#undef COHEAR_DELEGATE_CLASS_NAME
#undef COHEAR_DELEGATE_SEPARATOR
#undef COHEAR_DELEGATE_JOIN_MACRO
#undef COHEAR_DELEGATE_DO_JOIN
#undef COHEAR_DELEGATE_DO_JOIN2
