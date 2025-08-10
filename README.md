# SMfilePlayer 

SMfilePlater is a parser and playback engine for `.sm` rhythm game charts.

This project parses `.sm` files and plays them back perfectly. To achieve this the project includes:

- Chart parsin logic

- Playback engine with timing synchronization

## Why I Built This

I play a lot of rythm games that use `.sm` charts. I searched online but couldn’t find a bot like this, so I decided to build one myself. 

## Usage

 **Build the bot:**
   ```sh
   g++ -std=c++17 -I./Include -o EtternaBot src/main.cpp src/Parser.cpp src/Bot.cpp
   ```

 **Prepare your chart:**  
   Place your `.sm` file in the `Charts/` directory and update the path in `main.cpp`.

 **Run the bot:**  
   ```sh
   ./EtternaBot
   ```
## Limitations

- Etterna/StepMania implement anti-cheat measures that block simulated input, so this bot cannot be used in-game directly.

- To work around this, hardware macro devices can emulate real keyboard input, making the bot’s output indistinguishable from human input.

## How It Works 
- Chart Parsing: Reads a `.sm` and for each column in the chart it calculates the precise timing of the note and which rows of arrows to hit.
- Bot (Playback + Input): Uses the parsed data to start a clock that tracks playback time. When the clock matches a note’s timing, the bot immediately simulates keypresses for all arrows in that note.

## What I Learned
From this project, I learned how to parse a custom file format with nested data structures, implement accurate playback timing and synchronization, apply basic music theory concepts for calculating note timings, and work with real-time input simulation.

