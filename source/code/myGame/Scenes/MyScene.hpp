#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{
	class FirstScene : public AbstractGame
	{
	public:
		FirstScene();
		virtual ~FirstScene();

		virtual void initialize();

	protected:
		virtual void _initializeScene();
		virtual void _render();
	private:
		DebugHud* _hud;

		void _updateHud();

		FirstScene(const FirstScene&);
		FirstScene& operator=(const FirstScene&);
	};
}

#endif //MYSCENE_HPP
