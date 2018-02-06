#ifndef ThirdScene_HPP
#define ThirdScene_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class ThirdScene : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		ThirdScene();
		virtual ~ThirdScene();

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

		ThirdScene(const ThirdScene&);
		ThirdScene& operator=(const ThirdScene&);
	};
}
#endif // ThirdScene_HPP
