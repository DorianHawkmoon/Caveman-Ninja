#pragma once
#ifndef ICOMPONENT_H
#define	ICOMPONENT_H

#include <string>
#include "Globals.h"
class Entity;

/*
*  Provides the interface for all properties
*/
class IComponent {
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
	virtual IComponent* makeClone() = 0;

	virtual bool start() { return true; };
	virtual update_status preUpdate() { return UPDATE_CONTINUE;  };
	virtual update_status update() { return UPDATE_CONTINUE; };
	virtual update_status postUpdate() { return UPDATE_CONTINUE; };
	virtual bool cleanUp() { return true; };

public:
	//TODO best site for the parent, risk of inapropiate delete
	Entity* parent;

private:
	/// The property ID assigned to this IProperty derived class
	const std::string componentID;
};

#endif	/* ICOMPONENT_H */