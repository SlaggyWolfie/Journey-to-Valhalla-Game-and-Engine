#ifndef SecondScene_HPP
#define SecondScene_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class SecondScene : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		SecondScene();
		virtual ~SecondScene();

		//override initialize so we can add a DebugHud
		virtual void initialize();

	protected:
		//override so we can construct the actual scene
		virtual void _initializeScene();

		//override render to render the hud as well.
		virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

		void _updateHud();

		SecondScene(const SecondScene&);
		SecondScene& operator=(const SecondScene&);
	};
}
#endif // SecondScene_HPP
