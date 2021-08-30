#ifndef TETROMINO_MANAGER_HPP
#define TETORMINO_MANAGER_HPP

#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "Tetromino.hpp"

enum class TetrominoType
{
    I = 0,
    J,
    L,
    O,
    S,
    T,
    Z,
    Count
};

class TetrominoManager
{
public:
    TetrominoManager();
    ~TetrominoManager();

    void Reset();
    Tetromino* GetNextTetromino();
    bool Continue();
    void DrawTetrominos(sf::RenderWindow *l_wind);

    Tetromino* GetCurrentTetromino();
    const std::vector<Tetromino*>& GetFrozenTetrominos();

private:
    Tetromino *m_currentTetromino;
    std::vector<Tetromino*> m_frozenTetrominos;
    std::array<std::unordered_map<Direction, std::array<sf::Vector2i, 4>>, 2> m_wallKickDataArr;

    Tetromino* CreateTetromino(TetrominoType l_type);
    void DeleteTetrominos();
};

#endif