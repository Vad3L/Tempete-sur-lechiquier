#include "MainEntity.hpp"

MainEntity::MainEntity(gf::ResourceManager& resources,GameData &gameData)
: m_gameData(gameData)
, m_card(resources)
{

}

void MainEntity::update([[maybe_unused]] gf::Time time) {
}

void MainEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

	constexpr float spaceBetweenCard = 0.208f;

	int cpt = 0;
	for (auto &c : m_gameData.m_main) {
		float y = -0.4f;
		if(cpt==m_hoverCard) {
			y = -0.5f;
		}
		m_card.draw(target,states,c, gf::Vector2f(-0.5f+spaceBetweenCard*cpt, y));
		cpt++;
	}
}

int MainEntity::getCardSelected([[maybe_unused]] gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) { //sizeWindows = CardsView

	int cardSelected = -1;
	//gf::Log::debug("y = %i et x = %i\n",mouseCoord.y, mouseCoord.x);
	if(mouseCoord.y < -120){
		gf::Log::debug("clique en dehors des cartes \n");
		return -1;
	}
	if(mouseCoord.x >= -600 && mouseCoord.x <= -400 ){
		cardSelected = 0;
	}
	if(mouseCoord.x >= -350 && mouseCoord.x <= -150){
		cardSelected = 1;
	}
	if(mouseCoord.x >= -100 && mouseCoord.x <= 100){
		cardSelected =  2;
	}
	if(mouseCoord.x >= 150 && mouseCoord.x <= 350){
		cardSelected  = 3;
	}
	if(mouseCoord.x >= 400 && mouseCoord.x <= 600){
		cardSelected = 4;
	}

	gf::Log::debug("carte selectionnée %i\n",cardSelected);
	return cardSelected;

}