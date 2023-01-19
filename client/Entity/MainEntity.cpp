#include "MainEntity.hpp"
#include "CardsEntity.hpp"

MainEntity::MainEntity(gf::ResourceManager& resources,GameData &gameData)
: m_gameData(gameData)
, m_card(resources,gameData)
{

}

void MainEntity::update([[maybe_unused]] gf::Time time) {
}

void MainEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

	constexpr float spaceBetweenCard = 0.208f;

	int cpt = 0;
	for (auto &c : m_gameData.m_main) {
		
		m_card.render(target,states,c,-0.5f+spaceBetweenCard*cpt,-0.4f);

		cpt++;
	}
}

int MainEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) { //sizeWindows = CardsView

	int cardSelected = -1;
	
	if(mouseCoord.y < -60){
		gf::Log::debug("clique en dehors des cartes \n");
		return -1;
	}
	if(mouseCoord.x >= -600 && mouseCoord.x <= -400 ){
		cardSelected = 0;
	}
	if(mouseCoord.x >= -360 && mouseCoord.x <= -160){
		cardSelected = 1;
	}
	if(mouseCoord.x >= -120 && mouseCoord.x <= 80){
		cardSelected =  2;
	}
	if(mouseCoord.x >= 120 && mouseCoord.x <= 320){
		cardSelected  = 3;
	}
	if(mouseCoord.x >= 360 && mouseCoord.x <= 560){
		cardSelected = 4;
	}

	gf::Log::debug("carte selectionnée %i\n",cardSelected);
	return cardSelected;

}