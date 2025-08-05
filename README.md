## EtternaBot is a Windows-based bot framework for parsing and playing StepMania/Etterna `.sm` chart files.

## How It Works

 **Chart Parsing:**  
   The bot reads a `.sm` file, extracts note rows, and calculates the exact time each note should be played.

 **Synchronization:**  
   Press F8 to start the bot. After a short delay, the bot sends Enter to start the chart in-game and begins playing notes in sync.

 **Key Simulation:**  
   The bot uses the Windows API to simulate keypresses for each note (Q, W, E, R for columns, Enter to start).

## Usage

 **Build the bot:**
   ```sh
   g++ -std=c++17 -I./Include -o EtternaBot src/main.cpp src/Parser.cpp src/Bot.cpp
   ```

 **Prepare your chart:**  
   Place your `.sm` file in the `Charts/` directory and update the path in `main.cpp` if needed.

 **Run the bot:**  
   ```sh
   EtternaBot.exe
   ```
## Limitations

-  Etterna/StepMania block simulated input for anti-cheat reasons.

- This limitation can be overcome by using a hardware macro device such as an Arduino or a programmable keyboard. These devices emulate real keyboard input at the hardware level, making their key presses indistinguishable from those of a physical keyboard and undetectable as simulated input by the game.
