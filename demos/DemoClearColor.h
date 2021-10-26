#pragma once

#include "Demo.h"

namespace demo {

	class DemoClearColor : public Demo
	{
	public:
		DemoClearColor();
		~DemoClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};

}