#pragma once
// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GetMacroOverLoad(_1, _2, _3, NAME, ...) NAME
#define wrap(...) __VA_ARGS__
#define COMMA ,

// add a member to your struct with [expression] = other variable&
#define addMemberName(memberName, expression) \
	constexpr t &memberName()                 \
	{                                         \
		return expression;                    \
	}                                         \
	constexpr const t &memberName() const     \
	{                                         \
		return expression;                    \
	}

#define addTypeSizes(structName)            \
	typedef structName##n<1> structName##1; \
	typedef structName##n<2> structName##2; \
	typedef structName##n<3> structName##3; \
	typedef structName##n<4> structName##4;

#define addTemplateSize(shortenedTemplateName, size) \
	template <typename t>                            \
	using shortenedTemplateName##t##size = shortenedTemplateName##tn<t, size>;

#define addTemplateSizes(shortenedTemplateName)       \
	addTemplateSize(shortenedTemplateName, 1)         \
		addTemplateSize(shortenedTemplateName, 2)     \
			addTemplateSize(shortenedTemplateName, 3) \
				addTemplateSize(shortenedTemplateName, 4)

#define addTemplateType(shortenedTemplateName, typeTemplateName, typeName)                                                 \
	template <fsize_t axisCount>                                                                                           \
	using shortenedTemplateName##typeTemplateName##n = shortenedTemplateName##tn<typeName, axisCount>;                     \
                                                                                                                           \
	addTypeSizes(shortenedTemplateName##typeTemplateName)                                                                  \
                                                                                                                           \
		template <fsize_t axisCount>                                                                                       \
		using c##shortenedTemplateName##typeTemplateName##n = const shortenedTemplateName##typeTemplateName##n<axisCount>; \
                                                                                                                           \
	addTypeSizes(c##shortenedTemplateName##typeTemplateName)

#define addTemplateTypes(shortenedTemplateName)                                         \
	template <typename t, fsize_t axisCount>                                            \
	using c##shortenedTemplateName##tn = const shortenedTemplateName##tn<t, axisCount>; \
	addTemplateSizes(shortenedTemplateName)                                             \
		addTemplateSizes(c##shortenedTemplateName)                                      \
			addTemplateType(shortenedTemplateName, , fp)                                \
				addTemplateType(shortenedTemplateName, i, int)                          \
					addTemplateType(shortenedTemplateName, l, long)                     \
						addTemplateType(shortenedTemplateName, b, bool)                 \
							addTemplateType(shortenedTemplateName, s, size_t)

// https://stackoverflow.com/questions/18290523/is-a-default-move-constructor-equivalent-to-a-member-wise-move-constructor
#define addDefaultConstructors(structName, constructorType)              \
	constructorType structName(const structName &other) = default;       \
	constructorType structName(structName &&other) = default;            \
	constructorType structName &operator=(structName &&other) = default; \
	constructorType structName &operator=(const structName &other) = default;

// newexpression should be a macro that creates a new instance with the same size as the argument
#define addOperator(o, newExpression, structType, otherStructType, functionType)                                                               \
	template <typename t2, typename resultType = std::enable_if_t<std::is_arithmetic_v<t2>, decltype(std::declval<t>() o std::declval<t2>())>> \
	functionType decltype(auto) operator o(const t2 &b) const                                                                                  \
	{                                                                                                                                          \
		newExpression(resultType, (*this)) auto resultPtr = result.begin();                                                                    \
		auto const &endPtr = end();                                                                                                            \
		for (auto thisPtr = begin(); thisPtr < endPtr; thisPtr++, resultPtr++)                                                                 \
		{                                                                                                                                      \
			*resultPtr = *thisPtr o b;                                                                                                         \
		}                                                                                                                                      \
		return result;                                                                                                                         \
	}                                                                                                                                          \
	template <typename t2, typename resultType = decltype(std::declval<t>() o std::declval<t2>())>                                             \
	functionType decltype(auto) operator o(const otherStructType &b) const                                                                     \
	{                                                                                                                                          \
		newExpression(resultType, b) auto resultPtr = result.begin();                                                                          \
		auto const &endPtr = end();                                                                                                            \
		auto bPtr = b.begin();                                                                                                                 \
		for (auto thisPtr = begin(); thisPtr < endPtr; thisPtr++, resultPtr++, bPtr++)                                                         \
		{                                                                                                                                      \
			*resultPtr = *thisPtr o(*bPtr);                                                                                                    \
		}                                                                                                                                      \
		return result;                                                                                                                         \
	}                                                                                                                                          \
	template <typename t2, typename resultType = std::enable_if_t<std::is_arithmetic_v<t2>, decltype(std::declval<t>() o std::declval<t2>())>> \
	functionType friend decltype(auto) operator o(const t2 &a, const structType &b)                                                            \
	{                                                                                                                                          \
		newExpression(resultType, b) auto resultPtr = result.begin();                                                                          \
		auto const &endPtr = b.end();                                                                                                          \
		for (auto bPtr = b.begin(); bPtr < endPtr; bPtr++, resultPtr++)                                                                        \
		{                                                                                                                                      \
			*resultPtr = a o(*bPtr);                                                                                                           \
		}                                                                                                                                      \
		return result;                                                                                                                         \
	}                                                                                                                                          \
	template <typename t2>                                                                                                                     \
	functionType structType operator o##=(const otherStructType &b)                                                                            \
	{                                                                                                                                          \
		auto const &endPtr = end();                                                                                                            \
		auto bPtr = b.begin();                                                                                                                 \
		for (auto thisPtr = begin(); thisPtr < endPtr; thisPtr++, bPtr++)                                                                      \
		{                                                                                                                                      \
			*thisPtr o## = (*bPtr);                                                                                                            \
		}                                                                                                                                      \
		return *this;                                                                                                                       \
	}                                                                                                                                          \
	template <typename t2>                                                                                                                     \
	functionType typename std::enable_if<std::is_arithmetic_v<t2>, void>::type operator o##=(const t2 &b)                                      \
	{                                                                                                                                          \
		for (t & val : (*this))                                                                                                                \
		{                                                                                                                                      \
			val o## = b;                                                                                                                       \
		}                                                                                                                                      \
	}

#define addOperators(newExpression, structType, otherStructType, functionType)             \
	addOperator(+, newExpression, structType, wrap(otherStructType), functionType)         \
		addOperator(-, newExpression, structType, wrap(otherStructType), functionType)     \
			addOperator(*, newExpression, structType, wrap(otherStructType), functionType) \
				addOperator(/, newExpression, structType, wrap(otherStructType), functionType)

#define addAssignmentOperator(structName, functionType) \
	functionType structName &operator=(structName copy) \
	{                                                   \
		this->swap(copy);                               \
		return *this;                                   \
	}
