#ifndef TSL_DOWNLOAD_ENTITY_H
#define TSL_DOWNLOAD_ENTITY_H

#include <atomic>

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Ref.h>

namespace tsl {

	class DownloadEntity: public gf::Entity {
		public:
			DownloadEntity(gf::Texture& tempTexture);

			void loadAnimation(std::vector<gf::Ref<gf::Texture>> textures);
			void changeFrame();
			
			void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

		private:
			gf::Texture& m_tempTexture;
			std::atomic_bool m_animationLoaded;

			std::vector<gf::Ref<gf::Texture>> m_textures;
			float m_percentage;
	};

} 

#endif // TSL_DOWNLOAD_ENTITY_H