#ifndef ABSTRACTBEHAVIOUR_HPP
#define ABSTRACTBEHAVIOUR_HPP

class GameObject;

/**
 * An AbstractBehaviour allows you to attach reusable behaviours to GameObjects (steering, rotating, billboarding, etc).
 * A behaviour is set on a GameObject, which in turn passes in a reference to itself through the setOwner method.
 * This way we can enforce that a Behaviour can never have an owner different from the object it has been attached to.
 *
 * The concept is similar to MonoBehaviours in Unity.
 */
class AbstractBehaviour
{
	public:

		AbstractBehaviour();
		virtual ~AbstractBehaviour() = 0;

        //for internal administration, do not use directly
        virtual void setOwner (GameObject* pGameObject);

        //behaviour should be able to update itself every step and MUST be implemented
		virtual void update(float pStep) = 0;

    protected:

		GameObject* _owner;

    private:

        //disallow copy and assignment
        AbstractBehaviour(const AbstractBehaviour&);
        AbstractBehaviour& operator=(const AbstractBehaviour&);

};

#endif // ABSTRACTBEHAVIOUR_HPP
