#include "TableBoardEntity.h"

namespace tsl {

    TableBoardEntity::TableBoardEntity(gf::ResourceManager& resources, GameModel &model)
    : m_model(model)
    , m_font(resources.getFont("fonts/DejaVuSans.ttf"))
    , m_backgroundTexture(resources.getTexture("images/ChessSheet.png"))
    , m_backgroundTexture2(resources.getTexture("images/ChessSheet2.png"))
    , m_backgroundTexture3(resources.getTexture("images/ChessSheet3.png"))
    {
        m_backgroundTexture.setSmooth(true);
        m_backgroundTexture2.setSmooth(true);
        m_backgroundTexture3.setSmooth(true);
    }

    void TableBoardEntity::update([[maybe_unused]] gf::Time time) {
    }

    void TableBoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(gf::Vector2i(500.f , 500.f));

        constexpr int numberPiece = (int)ChessPiece::MAX - (int)ChessPiece::MIN + 1;
        gf::Texture &texture = (m_model.theme == 1) ? m_backgroundTexture : (m_model.theme == 2) ? m_backgroundTexture2 : m_backgroundTexture3;

        gf::RectangleShape tableCloth(gf::Vector2f(500.f, 400.f));
        tableCloth.setAnchor(gf::Anchor::Center);
        tableCloth.setPosition(coords.getRelativePoint({0.f, 0.f}));
        tableCloth.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
        target.draw(tableCloth,states);

        
        std::string indication = m_model.getWord("No indication");
        
        switch (m_model.gamePhase.getCurrentPhase()){
            case Phase::AVANT_COUP :
                indication = m_model.getWord("Play a normal card or move");
                break;
            case Phase::APRES_COUP :
                indication = m_model.getWord("Play card");
                break;
            case Phase::COUP :
                indication = m_model.getWord("Play a normal move");
                break;
            case Phase::PAS_MON_TOUR :
                indication = m_model.getWord("Opponent's turn");
                break;
            default :
                break;
        }

        switch (m_model.gamePhase.getCurrentSubPhase()) {
            case SubPhase::CLIQUER_CASES:
                indication = m_model.getWord("Choose boxes on the board");
                break;
            default :
                break;
        }

        switch (m_model.chessStatus) {
            case ChessStatus::WIN:
                indication = m_model.getWord("You win");
                break;
            case ChessStatus::LOOSE:
                indication = m_model.getWord("You loose");
                break;
            case ChessStatus::EQUALITY:
                indication = m_model.getWord("Equality");
                break;
            case ChessStatus::NO_STARTED:
                indication = m_model.getWord("Game not started");
                break;
            case ChessStatus::SURRENDER:
                indication = m_model.getWord("You win by surrender");
                break;
            default :
                break;
        }
        

        gf::Text text(indication, m_font);
        text.setScale(0.5f);
        text.setPosition(gf::Vector2f(coords.getRelativePoint({0.f, -0.36f})));
        text.setAnchor(gf::Anchor::Center);
        (m_model.theme==1) ? text.setColor(gf::Color::Black) : text.setColor(gf::Color::Gray(0.8f)) ;
        target.draw(text, states);

        std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};
        
        int incr = (m_model.chessColor == ChessColor::WHITE) ? 7 : 0; 
        //print border of board with letter and number
        for(int i = 1 ; i <= 8;++i) { 
            // letter
            gf::Text text(letters[7 - incr], m_font);
            
            text.setScale(0.5f);
            text.setPosition({coords.getRelativePoint({-0.29f + (i-1) * 0.08f, 0.37f})});
            
            if(m_model.theme == 1) {
                text.setColor(gf::Color::fromRgba32(238,198,108));
            }else {
                text.setColor(gf::Color::fromRgba32(160,160,160));
            }
            
            target.draw(text);

            // number   
            text.setString(std::to_string(incr+1));
            text.setPosition(coords.getRelativePoint({-0.37f, -0.27f + (i-1) * 0.08f}));
            
            target.draw(text, states);
            (m_model.chessColor == ChessColor::WHITE) ? incr-- : incr++; 
        }
        
        // draw bin
        int tabW[numberPiece];
        int tabB[numberPiece];
        
        for(int i = 0 ; i < numberPiece ; ++i){
            tabB[i] = 0;
            tabW[i] = 0;
        }
        
        for(Piece &pi : m_model.board.m_bin) {
            gf::Sprite sprite;
            float i = (float)pi.getType();
            float j = (int)(pi.getColor())/4.f;
            
            sprite.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
            
            if(pi.getColor() == ChessColor::WHITE) {
                sprite.setPosition(coords.getRelativePoint({-0.43f - 0.027f * (tabW[(int)i]%4), -0.255f + (i-2) * 0.084f + 0.025f * (tabW[(int)i]/4)}));
                tabW[(int)i]+=1;
            }else {
                sprite.setPosition(coords.getRelativePoint({0.35f + 0.027f * (tabB[(int)i]%4), -0.255f + (i-2) * 0.084f + 0.025f * (tabB[(int)i]/4)}));
                tabB[(int)i]+=1;
            }
            
            sprite.setScale((1.f / 11.f));
            target.draw(sprite, states);
        } 
        
    }

}