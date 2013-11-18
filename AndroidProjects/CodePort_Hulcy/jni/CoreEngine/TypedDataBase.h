#pragma once
#include <typeinfo>

namespace Monky
{
	class TypedDataBase
	{
	public:
		virtual ~TypedDataBase() {}

		virtual const std::type_info& getTypeID() const = 0;
		virtual TypedDataBase* clone() const = 0;

	};
}
