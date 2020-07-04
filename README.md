# T.B.D 3D Game Engine
[![Go to video](https://img.youtube.com/vi/YBdEydwaXGs/0.jpg)](https://www.youtube.com/watch?v=YBdEydwaXGs)

## About the project
Based on the framework provided by Javier Agenjo, teacher of the UPF, for the class of Electronic Games.<br/>
The objetive of this class was to develop our own game engine, for a small game of our creation.<br/>

## Dependecies
- Javier Agenjo's framework, that wraps arorund SDL2 and some of OpenGL's functionality.
- SDL2.
- OpenGL.
- Bass Sound library.
- Coldet library, for collision detection.
- Tweeny, for interpolation and animation (but the functionality was scrapped in the last versions).
- Pathfinders library, for the A* implementation.

## The Game
This game was supposed to be a homage/slight rip-off of the more survival horror games, specifically the Resident Evil 4; but due to development problems, and
time limits, I didnt have time to do any level desing, so the video is more like a tech demo, than a gameplay example.
For good or for worse, this was also the first time that I moddeled using Blender, let alone do some texturing, but in the end it came out dececent, and all the visual assets where created by me (the sound assets where found using the [Freesound database](https://freesound.org), that I think is a project of the UPF.
The enemys are temporally disabled if you shoot them enough times, or instantly if you just headshot them.
The player alternates between First and Third person, only being able to shoot while of first person, where he move more slowly.

### Intersesting parts
Since this is the first time I seriusly devled into engine development, some mistakes where made along the way; and that is accentuated since it is also y first time using Data Oriented Programming in a project, but nevertheless, there are still some interesting parts that I would like to highlight. (Or this are just some parts that I had a lot of fun working in).

### [Enemies](https://github.com/JsMarq96/TBN_3D/blob/master/src/enemy_entity.cpp)
In the enemy's code, I choose to highlight, the AI code, located in the *update* method.
The code for the AI's behabiour is moddled after a state machine with 5 states:
  -Stopped: Traces a path to neighborring point, using the A* algorithm.
  -Roam: follows the path determinated by the stopped state.
  -Run after: if the player is in sight, just run after him.
  -Attack: if the enemy is near the player, tries to punch him.
  -Recovering: if the player has shot down the enemy, the enemy is faded away, while it is recovering.
  
The code for this method resultied in a spagetty mess, so I have to come back in the future to state machine moddeling problems...
Since we already have a 2D representation of the Map, for the enemys to roam in, in order to avoid multiple raycastings to the player and the enviorment, in order
to test the enemy-player visibility, so I just rasterized a line from the enemy to the player on the map, in order to test if there is anything between them.

### [Player](https://github.com/JsMarq96/TBN_3D/blob/master/src/game_player.cpp)
Here, the higlight is the interpolation between the first, third and death cameras. It is no something that is very difficult or compley, but a little bit of lineal interpolation goes a long way in the game feel department!

### [Animated Particle Systems](https://github.com/JsMarq96/TBN_3D/blob/master/src/particles/animation_particle.cpp)
A generic implementation of a 2D animated element, like the muzzle flash, and the blood splatters. The meat of this, is in the *render* function, where I deffined the plane and its UV coordinates depending on a the current frame.
This is one of the more cleaner and least complex DOP implementations of the project, but I feel that having a component for each one of the particles is a waste of resources and kinda overkill, (since not a lot of particles happend at the same time). Maybe it would have been more efficient to just wrap all particles in the same component...

## The Result
Special thanks to my brothe and parents, who where kind enought to my during the last minute crunch time; and my teacher Javier Agenjo, that introduced me to this amazing area of work, that made it so hard to dedicate time to the other classes...
I don't know if I will keep updating this engine, but I have plans for a 2D engine and some more advanced 3D graphics, so I think this is just the starting point!
