#include "Game.hpp"
#include "Constants.hpp"

Game::Game() : m_window(&m_tetroMgr), m_infoBox(m_level, m_score), m_tickTime(0.5f)
{
    Setup();
    RestartClock();
    if (m_tetrisTheme.openFromFile("./res/tetris.ogg"))
    {
        m_tetrisTheme.setLoop(true);
        m_tetrisTheme.play();
    }
}

Game::~Game() {}

void Game::Setup()
{
    m_tetroMgr.Reset();
    m_score = 0;
    m_level = 1;
    m_infoBox.UpdateValues(m_level, m_score);
    m_linesRemovedInLvl = 0;
    m_lost = false;
}

void Game::HandleInput()
{
    m_window.HandleEvents();
}

void Game::Update()
{
    if (m_elapsed >= m_tickTime)
    {
        Tick();
        m_elapsed -= m_tickTime;
    }
}

void Game::Tick()
{
    if(!m_tetroMgr.Continue())
    {
        m_window.SetDone(true);
    }
    int l_linesRemoved = m_tetroMgr.GetLinesRemoved();
    if (l_linesRemoved > 0)
    {
        ProcessChanges(l_linesRemoved);
        m_infoBox.UpdateValues(m_level, m_score);
    }
}

void Game::ProcessChanges(int l_linesRemoved)
{
    switch (l_linesRemoved)
    {
    case 1:
        m_score += 40 * m_level;
        break;
    case 2:
        m_score += 100 * m_level;
        break;
    case 3:
        m_score += 300 * m_level;
        break;
    case 4:
        m_score += 1200 * m_level;
        break;
    default:
        // A generous gift for a player who manages to do the impossible
        m_score += 40000 * m_level;
        break;
    }

    m_linesRemovedInLvl += l_linesRemoved;
    if (m_linesRemovedInLvl >= 5)
    {
        m_level += 1;
        m_linesRemovedInLvl = 0;

        // Change Tick Time
        if (m_level <= 5)
        {
            m_tickTime -= game_constants::k_tickTimeDecrement;
        }
    }
}

void Game::DrawScene()
{
    m_window.BeginDraw();
    m_tetroMgr.DrawTetrominos(m_window.GetRenderWindow());
    m_infoBox.Draw(m_window.GetRenderWindow());
    m_window.EndDraw();
}

void Game::RestartClock()
{
    m_elapsed += m_clock.restart().asSeconds();
}

Window* Game::GetWindow() { return &m_window; }