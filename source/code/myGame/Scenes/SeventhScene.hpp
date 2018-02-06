#ifndef SEVENTH_SCENE_HPP
#define SEVENTH_SCENE_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class SeventhScene : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		SeventhScene();
		virtual ~SeventhScene();

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

		SeventhScene(const SeventhScene&);
		SeventhScene& operator=(const SeventhScene&);
	};
}
#endif // SEVENTH_SCENE_HPP
