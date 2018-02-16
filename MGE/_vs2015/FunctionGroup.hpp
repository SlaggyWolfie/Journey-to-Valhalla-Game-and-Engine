#pragma once
#ifndef LOOP_HPP
#define LOOP_HPP

#include <vector>
#include <functional>
#include "GeneralHelpers.hpp"

namespace Engine
{
	namespace Utility
	{
		template<typename T, typename U = void, typename ... P>
		class FunctionGroup
		{
		public:
			FunctionGroup();
			virtual ~FunctionGroup();
			void subscribe(T obj, std::function<U(P ...)> func);
			void unsubscribe(T obj);
			bool isSubscribed(T obj);
			void execute(P... params);
			void operator()(P... params);
		protected:
			std::vector<T> _objects;
			std::vector<std::function<U(P ...)>> _functions;
		};

		template <typename T, typename U, typename ... P>
		FunctionGroup<T, U, P...>::FunctionGroup() :
			_objects(std::vector<T>()),
			_functions(std::vector<std::function<U(P...)>>())
		{
		}

		template <typename T, typename U, typename ... P>
		FunctionGroup<T, U, P...>::~FunctionGroup()
		{
			_objects.clear();
			_functions.clear();
		}

		template <typename T, typename U, typename ... P>
		void FunctionGroup<T, U, P...>::subscribe(T obj, std::function<U(P ...)> func)
		{
			if (isSubscribed(obj)) return;
			//if (std::find(_objects.begin(), _objects.end(), obj) != _objects.end()) return;

			_objects.push_back(obj);
			_functions.push_back(func);
		}

		template <typename T, typename U, typename ... P>
		void FunctionGroup<T, U, P...>::unsubscribe(T obj)
		{
			auto objectIter = std::find(_objects.begin(), _objects.end(), obj);
			if (objectIter == _objects.end()) return;

			const auto func = _functions[static_cast<int>(std::distance(_objects.begin(), objectIter))];
			List::removeFrom(_objects, obj);
			List::removeFrom(_functions, func);
		}

		template <typename T, typename U, typename ... P>
		bool FunctionGroup<T, U, P...>::isSubscribed(T obj)
		{
			return std::find(_objects.begin(), _objects.end(), obj) != _objects.end();
		}

		template <typename T, typename U, typename ... P>
		void FunctionGroup<T, U, P...>::execute(P... params)
		{
			for (auto func : _functions)
				func(&params...);
		}

		template <typename T, typename U, typename ... P>
		void FunctionGroup<T, U, P...>::operator()(P... params)
		{
			execute(params);
		}
	}
}

#endif //LOOP_HPP