#pragma once
#ifndef OBJECT_HPP
#define OBJECT_HPP

namespace Engine
{
	class Object
	{
		virtual void destroy() = 0;
	};
}

#endif