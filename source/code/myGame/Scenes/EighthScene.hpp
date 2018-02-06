#ifndef EIGHTH_SCENE_HPP
#define EIGHTH_SCENE_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class EighthScene : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		EighthScene();
		virtual ~EighthScene();

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

		EighthScene(const EighthScene&);
		EighthScene& operator=(const EighthScene&);
	};
}
#endif // EIGHTH_SCENE_HPP
