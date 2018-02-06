#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{
	class FirstSceneSP : public AbstractGame
	{
	public:
		FirstSceneSP();
		virtual ~FirstSceneSP();

		virtual void initialize();

	protected:
		virtual void _initializeScene();
		virtual void _render();
	private:
		DebugHud* _hud;;

		void _updateHud();

		FirstSceneSP(const FirstSceneSP&);
		FirstSceneSP& operator=(const FirstSceneSP&);
	};
}

#endif //MYSCENE_HPP