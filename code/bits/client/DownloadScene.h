	#ifndef TSL_DOWNLOAD_SCENE_H
	#define TSL_DOWNLOAD_SCENE_H

	#include <atomic>
	#include <memory>

	#include <gf/ResourceManager.h>
	#include <gf/Scene.h>

	#include "DownloadEntity.h"

namespace tsl {

	struct GameHub;

	class DownloadScene : public gf::Scene {
		public:
			DownloadScene(GameHub& game);

			void changeFrame();
			
		private:
			void doUpdate(gf::Time time) override;

			GameHub& m_game;

			DownloadEntity m_downloadEntity;
	};

}

#endif // TSL_DOWNLOAD_SCENE_H