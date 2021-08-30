#include "Tetromino.hpp"
#include "TetrominoManager.hpp"

Tetromino::Tetromino(TetrominoManager *l_tetrominoManager, const std::array<sf::Vector2i, 4> &l_squarePositions, std::unordered_map<Direction, std::array<sf::Vector2i, 4>> *l_wallKickData, sf::Vector2f l_centreOfRotation, sf::Color l_colour)
    : m_squarePositions(l_squarePositions), m_CoR(l_centreOfRotation), m_tetroMgr(l_tetrominoManager), m_facing(Direction::Up), m_wallKick(l_wallKickData)
{
    m_position = sf::Vector2i((game_constants::k_gridSize.x / 2) - 2, -2);

    m_rect.setFillColor(l_colour);
    m_rect.setSize(sf::Vector2f(game_constants::k_squareSize - 1, game_constants::k_squareSize - 1));
}

Tetromino::~Tetromino() {}

void Tetromino::Draw(sf::RenderWindow *l_win)
{
    for(auto itr = m_squarePositions.begin(); itr != m_squarePositions.end(); ++itr)
    {
        m_rect.setPosition
        (
            (m_position.x + itr->x) * game_constants::k_squareSize,
            (m_position.y + itr->y) * game_constants::k_squareSize
        );
        l_win->draw(m_rect);
    }
}

bool Tetromino::Fall()
{
    const std::vector<Tetromino*> &l_frozenTetrominos = m_tetroMgr->GetFrozenTetrominos();
    int originalY = m_position.y;
    m_position.y += 1;
    if (IntersectsTetromino() || BelowFloor())
    {
        m_position.y = originalY;
        return false;
    }
    return true;
}

void Tetromino::MoveLeft()
{
    int l_originalX = m_position.x;
    m_position.x -= 1;
    if (PassesWall() || IntersectsTetromino()) { m_position.x = l_originalX; }
}
void Tetromino::MoveRight()
{
    int l_originalX = m_position.x;
    m_position.x += 1;
    if (PassesWall() || IntersectsTetromino()) { m_position.x = l_originalX; }
}

void Tetromino::RotateCW()
{
    const double pi = std::acos(-1);
    std::array<sf::Vector2i, 4> originalSquares = m_squarePositions;
    for (int i = 0; i < m_squarePositions.size(); ++i)
    {
        sf::Vector2f offset(m_squarePositions[i].x - m_CoR.x, m_squarePositions[i].y - m_CoR.y);
        int projectedX = std::round(m_CoR.x + (std::cos(-pi / 2) * offset.x - std::sin(-pi / 2) * -offset.y));
        int projectedY = std::round(m_CoR.y - (std::sin(-pi / 2) * offset.x + std::cos(-pi / 2) * -offset.y));
        m_squarePositions[i] = sf::Vector2i(projectedX, projectedY);
    }
    // Perform necessary checks
    if (PassesWall() || IntersectsTetromino() || BelowFloor())
    {
        m_squarePositions = originalSquares;
        return;
    }
    m_facing = Direction((int(m_facing) + 1) % 4);
}

std::array<sf::Vector2i, 4> Tetromino::GetTilePositions()
{
    std::array<sf::Vector2i, 4> l_tilePositions;
    for (int i = 0; i < l_tilePositions.size(); ++i)
    {
        l_tilePositions[i] = m_position + m_squarePositions[i];
    }
    return l_tilePositions;
}

bool Tetromino::PassesWall()
{
    for (auto &m_worldSqrPos : GetTilePositions())
    {
        if (m_worldSqrPos.x < 0 || m_worldSqrPos.x >= game_constants::k_gridSize.x) { return true; }
    }
    return false;
}

bool Tetromino::BelowFloor()
{
    for (auto &m_worldSqrPos : GetTilePositions())
    {
        if (m_worldSqrPos.y >= (int)game_constants::k_gridSize.y) { return true; }
    }
    return false;
}

bool Tetromino::IntersectsTetromino()
{
    const auto &frozenTetrominos = m_tetroMgr->GetFrozenTetrominos();
    if (frozenTetrominos.empty()) { return false; }
    for (auto &frozenTetro : frozenTetrominos)
    {
        if (intersects(GetTilePositions(), frozenTetro->GetTilePositions())) { return true; }
    }
    return false;
}

bool Tetromino::TouchesCeiling()
{
    for (auto &m_worldSqrPos : GetTilePositions())
    {
        if (m_worldSqrPos.y <= 0) { return true; }
    }
    return false;
}

bool intersects(const std::array<sf::Vector2i, 4> &first, const std::array<sf::Vector2i, 4> &second)
{
    for (auto &posFirst : first)
    {
        for (auto &posSecond : second)
        {
            if (posFirst == posSecond) { return true; }
        }
    }
    return false;
}