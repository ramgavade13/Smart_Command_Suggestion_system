
# Smart Command Assistant

## Overview
This project implements a **Smart Command Suggestion Engine** for Windows using C++ and Trie data structure. It provides real-time command suggestions and executes commands, including opening apps, URLs, and performing system operations.

## Features
- Over 40 commands for real Windows apps and system functions.
- Real-time suggestions with Trie (autocomplete).
- Case-insensitive search.
- Actions: open apps, URLs, mail client, show time/date, system operations.

## Installation
1. Open `src/main.cpp` in Visual Studio or any C++ IDE on Windows.
2. Compile and run the project.
3. Type a command prefix to see suggestions.
4. Enter the number of the command to execute.

## Benefits for Users
- Quick access to apps and system functions without navigating menus.
- Useful for learning data structures (Trie) and system automation.
- Demonstrates practical real-time AI-like suggestions.

## Example Usage
- Type `op` → Suggestions: "open notepad", "open paint", "open powershell", etc.
- Type `sh` → Suggestions: "shutdown system", "share screen", etc.
- Choose number to execute the command.

## Project Structure
```
SmartCommandAssistant/
│
├─ src/           # C++ source code
│   └─ main.cpp
├─ docs/          # Documentation PDFs
├─ assets/        # Flow diagram image
└─ README.md      # This file
```
