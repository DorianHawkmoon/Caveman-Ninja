#pragma once
#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>
#include "Transform.h"
#include "Globals.h"
class Entity;

class SceneNode {
public:
	SceneNode(Entity* entity);
	~SceneNode();

	/**
	* Add a node to this node
	* @param child the child
	*/
	void addChild(SceneNode* child);
	SceneNode* addChild(Entity* child);

	/**
	* Remove the given node to this node
	* @param node the child to remove
	* @return the removed node
	*/
	SceneNode* removeChild(const SceneNode& node);

	/**
	* Get the absolute transform (position/rotation and scale) of the node
	* @return the absolute transform
	*/
	Transform getWorldTransform() const;


	void remove();

	/**
	* Elimina los nodos marcados para eliminar (sus entidades est�n eliminadas)
	*/
	void removeWrecks();
	/*
	* Devuelve si el nodo es marcado para eliminar
	* @return true si esta destruido
	*/
	virtual bool isMarkedForRemoval() const;
	/**
	* Devuelve si el nodo est� destruido
	* @return true si esta destruido
	*/
	virtual bool isDestroyed() const;


	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

private:
	/**
	* Vector of children nodes
	*/
	std::vector<SceneNode*> children;

	/**
	* The parent of this node
	*/
	SceneNode* parent;

	/**
	* The entity asociated
	*/
	Entity* entity;
};


#endif // !SCENE_NODE_H
