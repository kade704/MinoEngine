#pragma once

#include "AView.h"
#include <Mino/Manager/SceneManager.h>

namespace Panel
{
	class GameView : public AView
	{
	public:
        GameView(const std::string& p_title);

        virtual void Update(float p_deltaTime) override;

        virtual void _Render_Impl() override;
    
        bool HasCamera() const;

    private:
        SceneManager& m_sceneManager;
        bool m_hasCamera = false;
    };
}
        