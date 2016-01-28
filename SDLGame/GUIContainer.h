#pragma once
#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GUIComponent.h"
#include <vector>

namespace GUI {
	class GUIContainer : public GUIComponent {
	public:
		/**
		* Constructor
		*/
		GUIContainer();
		/**
		* Destructor
		*/
		virtual ~GUIContainer();
		/**
		* Añade un componente al contenedor
		* @param component componente a añadir
		*/
		void pack(GUIComponent* component);
		/**
		* Indica si es seleccionable el contenedor
		* @return true si es seleccionable
		*/
		virtual bool isSelectable() const;
		/**
		* Procesa la entrada
		* @param event evento a procesar
		*/
		virtual void handleEvent(const SDL_Event& event);
		/**
		* Limpia la seleccion del contenedor
		*/
		void clearSelection();

		void draw(const GUITransform& states) const;

		virtual bool start();

		virtual update_status preUpdate();

		virtual update_status update();

		virtual bool cleanUp();

	private:
		/**
		* Lista de componentes hijos
		*/
		std::vector<GUIComponent*> children;
		/**
		* índice del hijo seleccionado
		*/
		int selectedChild;

		/**
		* Indica si tiene algún hijo seleccionado
		* @return true si hay hijo seleccionado
		*/
		bool hasSelection() const;
		/**
		* Selecciona un hijo concreto
		* @param index el hijo a seleccionar
		*/
		void select(unsigned int index);
		/**
		* Selecciona el siguiente hijo
		*/
		void selectNext();
		/**
		* Selecciona el anterior hijo
		*/
		void selectPrevious();
	};
}

#endif // !GUI_CONTAINER_H