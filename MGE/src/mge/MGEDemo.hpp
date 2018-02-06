#ifndef MGEDEMO_HPP
#define MGEDEMO_HPP

#include "mge/core/AbstractGame.hpp"
class DebugHud;

namespace demo
{

	/**
	 * An example subclass of AbstractGame showing how we can setup a scene.
	 */
	class MGEDemo : public AbstractGame
	{
		//PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();

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

		MGEDemo(const MGEDemo&);
		MGEDemo& operator=(const MGEDemo&);
	};
}
#endif // MGEDEMO_HPP
