English | [Français](README_FR.md)

# 🃏 **Planning Poker**
![License](https://img.shields.io/badge/License-UNLICENSE-red) ![Latest Version](https://img.shields.io/badge/Version-1.0.0-blue) ![OS](https://img.shields.io/badge/OS-Windows%2FmacOS%2FLinux-green)

Planning Poker is a fun way to estimate the development effort of features. Its simple and sleek interface allows teams to easily estimate the time required for desired tasks.

## 🎓 School Project

This is a small project made for [Université Lumière Lyon 2](https://www.univ-lyon2.fr/).
Named under the code name project CAPI, we were tasked with creating a project planning application in the desired programming language while implementing agile design practices.

## 🔧 Features

- **Intuitive User Interface:** A simple and ergonomic interface to facilitate session management.
- **Card Selection:** Choose from a selection of different cards for voting. A coffee card is also included, allowing the session to be paused.
- **Session Modes:** Two game modes are currently available. "Strict," where players must all agree on task estimates, and "Average," which starts in strict mode and then averages the players' votes.
- **JSON:** JSON format is used for data storage. This format's advantage is its potential compatibility with other applications in the future.
- **JSON Viewer:** An integrated JSON viewer allows you to easily view the results of your Planning Poker sessions.

## 🚀 Usage

1. **Create a JSON File:** Create a JSON file by naming it and specifying its tasks.
2. **Select a JSON File:** Choose a JSON file. If it's a JSON that's already been completed (the planning poker session has already been run and is finished), we won't be able to launch a session.
3. **Start Sessions:** Launch a session and enter the participants.
4. **Task Estimation:** Players estimate tasks one by one. They can use the coffee card at any time to pause the program.
5. **End the Session:** When consensus is reached, the average (or other depending on the chosen mode) is recorded in the JSON. Once all tasks are estimated, the JSON is finalized.

## 🔢 Versioning

This project follows a [semantic versioning](https://semver.org/) scheme.

## 🤝 Credits
This project was created by:
- BESSAD Tarek
- JOURNOUD Lucas

## 📄 License

This software is under [Unlicense](https://unlicense.org/), with terms available in [UNLICENSE.txt](UNLICENSE.txt).

## 📜 Acknowledgments

We especially thank our professor of agile project design, Mr. Valentin Lachand-Pascal.
