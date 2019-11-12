#include "Snake/Game.h"

Game::Game()
{
  m_world   = new World;
  m_window  = new Window;
  m_manager = new AssetManager;
}

Game::~Game()
{
  delete m_world;
  delete m_window;
  delete m_manager;
}

void Game::initialize()
{
  m_manager->addSound(FOOD_EAT);
  m_manager->addTexture(BACKGROUND_IMAGE);
  m_manager->addFont(MAIN_FONT);

  m_text.setFont(m_manager->getFont(MAIN_FONT));
  m_text.setCharacterSize(18);
  m_text.setFillColor(sf::Color::White);
  m_text.setPosition(50,50);

  m_food_eaten.setBuffer(m_manager->getSound(FOOD_EAT));
  m_isEnd = false;
  m_world->setEntitySize(40);
  m_window->setFrameRate(60);
  m_window->setWidth(1280);
  m_window->setHeight(640);
  m_window->setName("Snake");
  m_window->toggleFullScreen();
  m_window->create();
}

void Game::handleInput()
{
  sf::Event event;

  m_window->self()->pollEvent(event);

  if(event.type == sf::Event::Closed)
  {
    m_isEnd = true;
    m_window->self()->close();
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_world->snake().getCalculatedDirection() != Direction::DOWN)
  {
    m_world->snake().setDirection(Direction::UP);
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_world->snake().getCalculatedDirection() != Direction::UP)
  {
    m_world->snake().setDirection(Direction::DOWN);
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_world->snake().getCalculatedDirection() != Direction::RIGHT)
  {
    m_world->snake().setDirection(Direction::LEFT);
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_world->snake().getCalculatedDirection() != Direction::LEFT)
  {
    m_world->snake().setDirection(Direction::RIGHT);
  }
}

void Game::update()
{
  m_messages.clear();
  m_messages.push_back("Snake Position: (" + toString(m_world->snake().getPosition().x) + "," + toString(m_world->snake().getPosition().y) + ")");
  m_messages.push_back("Food Position: (" + toString(m_world->food().getPosition().x) + "," + toString(m_world->food().getPosition().y) + ")");
  m_messages.push_back("Lives Remaining: " + toString(m_world->snake().getLives()));
  
  m_world->snake().move();

  if(m_world->snake().getPosition().x == m_world->food().getPosition().x && m_world->snake().getPosition().y == m_world->food().getPosition().y)
  {
    m_food_eaten.play();
    m_world->food().generate(m_world->snake());
    m_world->snake().grow();

    //m_messages.push_back("Food eaten at: (" + toString(m_world->snake().getPosition().x) + "," + toString(m_world->snake().getPosition().y) + ")");
  }

  if(!m_world->snake().getLives())
  {
    m_world->snake().halt();
    m_isEnd = true;
    return;
  }




  return;
}

void Game::render()
{
  std::string result;
  m_window->erase(sf::Color::Black);
  m_world->render(*m_window->self());
  for(int i = 0; i<m_messages.size(); i++)
  {
    result += m_messages[i] + "\n";
  }
  m_text.setString(result);
  m_window->render(m_text);
  m_window->show();
  return;
}

std::string Game::toString(const int& l_val)
{
  std::stringstream converter;
  converter<<l_val;
  std::string result;
  converter>>result;
  return result;
}
