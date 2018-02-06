#ifndef FifthScene_HPP
#define FifthScene_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace MyGame
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class FifthScene : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		FifthScene();
		virtual ~FifthScene();

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

		FifthScene(const FifthScene&);
		FifthScene& operator=(const FifthScene&);
	};
}
#endif // FifthScene_HPP
