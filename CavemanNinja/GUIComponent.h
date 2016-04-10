#pragma once
#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "GUITransform.h"
#include "SDL\SDL.h"
#include "IUpdater.h"

namespace GUI {
	class GUIComponent : public IUpdater {
	public:
		GUIComponent();
		virtual ~GUIComponent();
		/**
		* Devuelve si el componente es seleccionable
		* @return true si es seleccionable
		*/
		virtual bool isSelectable() const = 0;
		/**
		* Devuelve si el componente est� seleccionado
		* @return true si est� seleccionado
		*/
		bool isSelected() const;
		/**
		* Selecciona el componente
		*/
		virtual void select();
		/**
		* Deselecciona el componente
		*/
		virtual void deselect();
		/**
		* Devuelve si el componente est� activa
		* @return true si est� activo
		*/
		virtual bool isActive() const;
		/**
		* Activa el componente
		*/
		virtual void activate();
		/**
		* Desactiva el componente
		*/
		virtual void deactivate();
		/**
		* Maneja los eventos capturados
		* @param event evento capturado
		*/
		virtual void handleEvent(const SDL_Event& event) = 0;

		virtual void draw(const GUITransform& state) const=0;

		/**
		* Transform
		*/
		GUITransform transform;


	private:
		/**
		* Para comprobar si est� seleccionado
		*/
		bool selected;
		/**
		* Para comprobar si est� activo
		*/
		bool actived;
	};

}
#endif // !GUI_COMPONENT_H