 # 📚 About This Project

This project was originally developed as part of a university assignment at the University of the West of England (UWE) 🏫 and is now shared as part of my personal portfolio and resume.

🛠️ It's here to showcase my work and to help others learn — you're welcome to explore the structure, coding style, and problem-solving approach for educational purposes.

🧩 Note: The browser HTML and CSS template was provided by the university. I was responsible only for the game creation and core logic development.

🚫 Please don’t copy or submit it as your own — that would go against academic integrity policies and could be considered plagiarism.

👍 Feel free to use it for inspiration, learn from it, and use what you learn to build something even better!

<br>

# 🎮 POP Snake Raylib Game

🧩 Simple starter application for a C assignment.

## 🚀 Game Features

* 🐍 Classic Snake gameplay with smooth controls  
* 🎨 User can change the snake’s color dynamically  
* 🍎 Snake grows longer each time it eats food, and its color changes accordingly  
* 👀 Unique snake design with cool eyes and mouth for a fun look  
* 🎨 Unique game board pattern that adds visual interest  
* 🎨 Background visuals present in the menu for better aesthetics  
* 🔊 Sound effects for eating food and collisions  
* ⚠️ Collision detection with walls and the snake’s own body  
* 📊 Score tracking with clear game over conditions  
* 🎮 Responsive input handling for an engaging experience

<br>

## 📸 Screenshots:

### 🏠 Main Menu
![Menu Screen](assets/Main_Menu.png)

### 🕹️ Gameplay
![Gameplay Screen](assets/Gameplay.png)

### ❌ Game Over Menu
![Game Over Screen](assets/Game_Over_Menu.png)

### ⚙️ Options 
![Options Screen](assets/Options.png)




**🔄 Clone this repository ⚠️ Important: You must clone this repository first before doing anything else!**
```bash
https://github.com/MohamedHamouda23/Word-games
```
# 🛠️ Building

To build run the command:

```bash
/opt/pop/bin/build-wasm.sh src/main.c
```


📁 This will generate a directory *out* with the WASM and index.html files for the 
Raylib program.

# 🚀 Running

The very first time you run a POP WASM application you must run the command:

```bash
/opt/pop/bin/allocate_port.sh
```

💡 You might need to start a new terminal instance for the update to take effect.
To check that everything is fine run the command:

```bash
echo $MY_PORT
```

🔢 This should output a 5 digit number.


To run the Raylib program in *out* simply run the command:

```bash
/opt/pop/bin/run-wasm.sh
```

🌐 This will run a web server that serves the *out* on the port you allocated above. This is forwarded from the 
remote server to your local machine, which means you can simply open the corresponding web page within a browser 
on your local machine using the address:

```bash
localhost:XXXXX
```

🧭 where *XXXXX* is the port number you allocated above.
