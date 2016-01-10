#pragma once
#ifndef ICOMPONENT_H
#define	ICOMPONENT_H

#include <string>
#include "Globals.h"
#include "IUpdater.h"
#include "CollisionListener.h"
class Entity;

/*
*  Provides the interface for all properties
*/
class IComponent : public IUpdater, public CollisionListener {
public:
	/**
	* IProperty default constructor
	* @param[in] theType of property this property represents
	* @param[in] thePropertyID to use for this property
	*/
	IComponent(const std::string thePropertyID);

	/**
	* IProperty destructor
	*/
	virtual ~IComponent();

	void setParent(Entity* entity);

	/**
	* GetID will return the Property ID used for this property.
	* @return the property ID for this property
	*/
	const std::string getID(void) const {
		return componentID;
	}

	bool isEqual(const IComponent* another);

	/**
	* MakeClone is responsible for creating a clone of this IProperty
	* derived class and returning it as part of the Prototype and Instance
	* system. The value of the Property will also be copied into the clone.
	* @return pointer to the IProperty derived class clone that was created
	*/
	virtual IComponent* makeClone() const = 0;

	virtual void onCollisionEnter(const Collider* self, const Collider* another) {};
	virtual void onCollisionExit(const Collider* self, const Collider* another) {};
	virtual void onCollisionStay(const Collider* self, const Collider* another) {};

	void enable() {
		if (!componentEnabled) {
			componentEnabled = true;
			start();
		}
	}

	void disable() {
		if (componentEnabled) {
			componentEnabled = false;
			toClean = true;
		}
	}

	bool isEnable() {
		return (componentEnabled || toClean);
	}

protected:
	Entity* parent;
	bool componentEnabled;
	bool toClean;

private:
	/// The property ID assigned to this IProperty derived class
	const std::string componentID;
};

#endif	/* ICOMPONENT_H */