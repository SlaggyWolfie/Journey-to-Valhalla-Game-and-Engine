#pragma once
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Manager;
		class Renderer;

		class RenderManager// : public Manager
		{
		public:
			RenderManager();
			~RenderManager();
			void addRenderer(Renderer* renderer);
			void removeRenderer(Renderer* renderer);
		protected:
		private:
			void render(float deltaTime);
			std::vector<Renderer*> _renderers;
		};
	}
}

#endif //RENDER_MANAGER_HPP