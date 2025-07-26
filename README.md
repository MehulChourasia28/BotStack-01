# BotStack 01 - Russian Roulette Bot Game

A strategic turn-based game implementation where players (bots or humans) face off in a Russian Roulette-style challenge with tactical items and multiple rounds. This project was developed as part of a competitive programming exercise.

## 🎮 Game Overview

This is a variation of Russian Roulette where two players take turns with a gun loaded with both live and blank bullets. Players can choose to shoot themselves or their opponent, use strategic items, and employ various tactics to win rounds.

### Game Rules

- **Objective**: Be the first player to win 2 out of 3 rounds
- **Health**: Each player starts each round with 3 health points
- **Gun**: Loaded with 3-7 bullets (mix of live and blank rounds)
- **Turns**: Players alternate turns, deciding whether to shoot themselves or their opponent
- **Items**: Players receive strategic items that can change the game state
- **Reloads**: Gun can be reloaded up to 3 times per round

### Shooting Mechanics

- **Shoot Self with Blank**: You continue your turn
- **Shoot Self with Live**: You lose 1 health, opponent gets the turn
- **Shoot Opponent with Live**: Opponent loses 1 health, opponent gets the turn
- **Shoot Opponent with Blank**: No damage, opponent gets the turn

### Strategic Items

Players can use various items to gain tactical advantages:

| Item | Effect |
|------|--------|
| **Peek Current** | See if the next bullet is live or blank |
| **Eject Current** | Remove the current bullet from the gun |
| **Skip Enemy** | Skip opponent's next turn |
| **Health Kit** | Gain one health point |
| **Reset Gun** | Empty and reload the gun with new bullets |

## 🏗️ Project Structure

```
Final/
├── main.c          # Core game engine and logic
├── header.h        # Data structures, enums, and function declarations
├── player1.c       # AI bot implementation (strategic)
├── player2.c       # Second player (bot or human interface)
├── cmds.txt        # Compilation commands and options
├── run.exe         # Compiled executable
└── README.md       # This file
```

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- Terminal/Command Prompt

### Compilation Options

The game supports different modes of play:

#### Bot vs Human Player
```bash
gcc main.c player1.c player2.c -o run -DPLAYER_MODE
```

#### Bot vs Bot
```bash
gcc main.c player1.c player2.c -o run
```

#### Additional Compilation Flags

- `-DNOPAUSE`: Disable the pause between turns (useful for bot vs bot)
- `-DDEBUG`: Enable debug output showing current bullets in the gun

Example with multiple flags:
```bash
gcc main.c player1.c player2.c -o run -DPLAYER_MODE -DNOPAUSE
```

### Running the Game

After compilation, run the executable:
```bash
./run        # Linux/Mac
run.exe      # Windows
```

## 🤖 Bot Implementation

### Player 1 Bot (Advanced AI)

The Player 1 bot (`player1.c`) implements a sophisticated strategy with two main modes:

- **Defensive Mode**: Activated when health is low (1 HP)
  - Prioritizes survival and health restoration
  - Uses peek items to avoid live bullets
  - Employs eject items when live bullet probability is low

- **Aggressive Mode**: Used when health is higher
  - Focuses on eliminating the opponent
  - Uses skip items for double-shot opportunities
  - Calculates probabilities to optimize bullet usage

### Player 2 Bot (Simple AI)

The Player 2 bot (`player2.c`) has dual functionality:
- **Bot Mode**: Simple AI that always shoots the opponent
- **Human Mode**: Interactive interface when compiled with `-DPLAYER_MODE`

## 🔧 Customizing Bots

To create your own bot strategy, modify the functions in `player1.c` or `player2.c`:

### Key Functions to Implement

```c
// Decide whether to abstain from the round
int player_abstain(Player self, Player other, Gun state, int opponentRequest);

// Choose which item to use (if any)
ItemAction player_item(Player self, Player other, Gun state);

// Decide who to shoot
TurnAction player_action(Player self, Player other, Gun state, Bullet nextBullet);
```

### Available Data Structures

```c
typedef struct Player {
    int rounds;    // Rounds won
    int lives;     // Current health
    Item item1;    // First available item
    Item item2;    // Second available item
} Player;

typedef struct Gun {
    int currentBullets;      // Total bullets remaining
    int currentLiveBullets;  // Live bullets remaining
} Gun;
```

## 🎯 Strategy Tips

### For Bot Development

1. **Probability Calculation**: Use `state.currentLiveBullets / state.currentBullets` to calculate live bullet probability
2. **Health Management**: Consider both your health and opponent's health in decision making
3. **Item Synergy**: Combine items strategically (e.g., Peek + appropriate shooting decision)
4. **Endgame Scenarios**: Implement specific logic for winning conditions

### Advanced Tactics

- **Double Shot Combo**: Use Skip Enemy item, then shoot opponent twice in a row
- **Safety Peek**: Use Peek Current when unsure, then make informed shooting decision
- **Health Kit Timing**: Use health kits strategically, not just when low on health
- **Gun Reset**: Use when bullet distribution is unfavorable

## 🏆 Win Conditions

The game ends when:
1. **Player Wins 2 Rounds**: First to win 2 rounds wins the game
2. **Double Abstention**: If both players abstain twice, the game ends in a draw
3. **Maximum Rounds**: After 3 rounds with no clear winner

## 🔍 Debugging

Enable debug mode to see the current bullet sequence:
```bash
gcc main.c player1.c player2.c -o run -DDEBUG
```

This will display the current bullets in the gun, helping you understand bot decisions and test strategies.

## 📊 Game Statistics

The game tracks and displays:
- Rounds won by each player
- Health status throughout the game
- Item usage and effects
- Bullet information (in debug mode)

## 🤝 Contributing

To contribute to this project:
1. Fork the repository
2. Create your bot implementation
3. Test against existing bots
4. Submit improvements or new strategies

## 📝 Notes

- The game uses `srand(time(NULL))` for randomization, ensuring different outcomes each game
- Gun bullet distribution is carefully balanced to maintain fair gameplay
- The abstention mechanic adds an additional layer of strategy for difficult situations

---

**Good luck creating your winning bot strategy!** 🎲 
