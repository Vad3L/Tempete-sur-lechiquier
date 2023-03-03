#ifndef TSL_DOWNLOAD_ENTITY_H
#define TSL_DOWNLOAD_ENTITY_H

#include <atomic>

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Ref.h>

#include "GameModel.h"

namespace tsl {

	class DownloadEntity: public gf::Entity {
		public:
			DownloadEntity(gf::ResourceManager& resources, GameModel& model);

			void changeFrame();			
			
			void update([[maybe_unused]] gf::Time time) override;
			void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

		private:
			float m_percentage;
			GameModel& m_model;
			
			std::vector<gf::Ref<gf::Texture>> m_textures;

			gf::Texture& m_backgroundTexture;
			gf::Texture& m_title;

			gf::Font& m_font;
			
			std::string m_points;
			gf::Time m_interval;
	};

} 

#endif // TSL_DOWNLOAD_ENTITY_H