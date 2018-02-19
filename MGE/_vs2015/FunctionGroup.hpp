#pragma once
#ifndef FUNCTION_GROUP_HPP
#define FUNCTION_GROUP_HPP

#include <utility>
#include <vector>
#include <functional>
#include "GeneralHelpers.hpp"

namespace Engine
{
	namespace Utility
	{
		template<typename OwnerType, typename ReturnType = void, typename ... ParameterType>
		class FunctionGroup
		{
		public:
			FunctionGroup();
			explicit FunctionGroup(std::function<bool(OwnerType)> predicate);
			~FunctionGroup();
			void subscribe(OwnerType obj, std::function<ReturnType(ParameterType ...)>& func);
			void unsubscribe(OwnerType obj);
			bool isSubscribed(OwnerType obj) const;
			void execute(ParameterType... params) const;
			void operator()(ParameterType... params) const;
			std::vector<OwnerType>& getObjects();
		private:
			std::vector<OwnerType> _objects;
			std::vector<std::function<ReturnType(ParameterType ...)>*> _functions;
			bool predicate(OwnerType obj) const;
			bool _predicateIsSet;
			std::function<bool(OwnerType)> _predicate;
		};

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		FunctionGroup<OwnerType, ReturnType, ParameterType...>::FunctionGroup() :
			_objects(std::vector<OwnerType>()),
			_functions(std::vector<std::function<ReturnType(ParameterType ...)>*>()),
			_predicateIsSet(false), _predicate()
		{
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		FunctionGroup<OwnerType, ReturnType, ParameterType...>::FunctionGroup(std::function<bool(OwnerType)> predicate) :
			_objects(std::vector<OwnerType>()),
			_functions(std::vector<std::function<ReturnType(ParameterType ...)>*>()),
			_predicateIsSet(true), _predicate(std::move(predicate))
		{
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		FunctionGroup<OwnerType, ReturnType, ParameterType...>::~FunctionGroup()
		{
			_objects.clear();
			for (auto func : _functions)
				delete func;
			_functions.clear();
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		void FunctionGroup<OwnerType, ReturnType, ParameterType...>::subscribe(OwnerType obj, std::function<ReturnType(ParameterType ...)>& func)
		{
			if (isSubscribed(obj)) return;
			//if (std::find(_objects.begin(), _objects.end(), obj) != _objects.end()) return;

			_objects.push_back(obj);
			_functions.push_back(&func);
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		void FunctionGroup<OwnerType, ReturnType, ParameterType...>::unsubscribe(OwnerType obj)
		{
			auto objectIter = std::find(_objects.begin(), _objects.end(), obj);
			if (objectIter == _objects.end()) return;

			const auto func = _functions[static_cast<int>(std::distance(_objects.begin(), objectIter))];
			List::removeFrom(_objects, obj);
			List::removeFrom(_functions, func);
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		bool FunctionGroup<OwnerType, ReturnType, ParameterType...>::isSubscribed(OwnerType obj) const
		{
			return std::find(_objects.begin(), _objects.end(), obj) != _objects.end();
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		void FunctionGroup<OwnerType, ReturnType, ParameterType...>::execute(ParameterType... params) const
		{
			//for (auto func : _functions)
			for (size_t i = 0; i < _objects.size(); i++)
				if (predicate(_objects[i])) (*_functions[i])(&params...);
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		void FunctionGroup<OwnerType, ReturnType, ParameterType...>::operator()(ParameterType... params) const
		{
			execute(params);
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		std::vector<OwnerType>& FunctionGroup<OwnerType, ReturnType, ParameterType...>::getObjects()
		{
			return _objects;
		}

		template <typename OwnerType, typename ReturnType, typename ... ParameterType>
		bool FunctionGroup<OwnerType, ReturnType, ParameterType...>::predicate(OwnerType obj) const
		{
			if (_predicateIsSet) return _predicate(obj);

			return true;
		}
	}
}

#endif //LOOP_HPP