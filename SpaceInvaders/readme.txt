######  STAGE ONE  ######
The current assignment implements a space ship that moves right and left + shoots.

The first 3 lines of the config take the size, x position and instructions.
The instructions are seperated by ",".


The Config has been implemented with a singleton design pattern.
The Bullet has been implemented with a builder design pattern.



The extensions implemented were:

- According to the size of the ship, the game screen is adjusted and so is the starting y position.

- When a bullet is shot, there is a laser beam noise made (with QSound).

- The 4th additional line in the config file is the number of frames per second and can be modified for each individuals experience.

- I have attempted to align the bullet to project from the middle of the ship, regardless of what size the ship is or starting x.



######  STAGE TWO  ###### 
Cool dogs.
CONFIG FILE SHOWCASE:
 config - is the one used for demo demonstrating everything, however aliens shoot quite a bit so it may close.
		- 'incorrect' config input is used to show error handling
 confg dmo - same as above
 config simple - shows 1 swarm
 config friendly - shows friendly fire of aliens against aliens.
 config simple alien crash, crash 2 - shows that aliens colliding with player shuts the game safely

Fixes:
- Fixed the previous extension where bullets were NOT ACTUALLY shooting from the middle of the ship :( This was due to 1) incorrect maths with magic numbers 2) the images had transparent pixel borders which would inevitably stuff up the maths in the first place, so I cropped them out in Photoshop.
- Fixed some magic numbers
- Fixed deletion and config fixes e.g., one List loop where they deleted items while iterating, without backwards iteration (without using a foreach loop) PS Dogs are really cool.
- Made it so ships and aliens cannot go out of the canvas.
- Base class now has a collision(Base &item) function, since QRect / QGraphicsScene is not implemented

Information:
- Config fixed s.t., it can read other [headers] (and can still read the old config). A bit confusing due to the assignment requirement of needing backward compatibility, but basically its like recursive.
- Used composite structural design for swarms of aliens. 
	Alien is a leaf, Swarm is a composite. 
- Made the ship instructions loop so it moves forever until game close / lose (not necessary for Stage 3)
- Aliens use the same bullet builder class as Ship, just different parameters 
- Aliens have a builder. If you want to make more aliens, please look at AlienBuilder
- Added 'friendliness' for bullets, used later for checking collision.
	- if you want aliens to shoot "Player" bullets for some reason (e.g., kill other aliens) try passing a special "type" of bullet to its builder. See Hunter class.

Other Notes FOR STAGE 3:
- For game scoreboard in part 3, take a look at the Menu class :)
- All dogs on earth wish that this was helpful for you.

Extensions:
	Aliens: (AKA dog haters)
	- Angled bullets - rotates the bullet image in bullet builder.
		> When ship or alien makes a bullet,
		need to tell bulletbuilder if they want to shoot angled.
	- Randomised shooting times (with given range at config)
		> Also shoots a random type of bullet
		(BUT YOU MAY REMOVE THIS, It's just to showcase angles)
	- Aliens that hunt the Player. 
		> And, aliens that are not so good at hunting. [hunter, dumb]
		> Normal, non hunting aliens are [red, blue]
	- REACTIONS: Aliens can react before they die.
		> Currently, Hunter aliens will shoot three bullets that can harm the player.
		> Dumb aliens will shoot three bullets that harm aliens.
	- Press 'P' to pause the game
	- Pausing displays the score button (it's a menu, but only 'score' menu is implemented)
		- score is opened with said button
	- Made so ships can't go out of the canvas area.
	- Player ship dies on contact with alien.

######  STAGE THREE  ###### 
Controlling the ship using left arrow/right arrow/space or mouse added. To change if ship should be controlled by keyboard/mouse/config put control=keyboard,control=mouse or control=config into config file.
Levels are introduced and automatically advances to the next if the last alien dies. Levels are read from their respective config file (config1.txt,config2.txt etc.) and if there is no more levels, it goes back to the first level and score is kept.

Pause menu improved:
Scoreboard automatically displayed without clicking a button.
Restart button added to restart the game, reset the current score, revert to first level and score will not be added to scoreboard.
Settings menu to change speed of all elements of the game and change background colour.
Death of ship no longer exits game, instead brings up pause menu and shows that ship has died.

scores.txt file stores up to 5 high scores, format of scores are name(string) followed by a single comma followed by the score(int). eg:
BOB,12

State design pattern used for the ship, if there are more than 3 aliens remaining then super state is enabled.
Super state shoots three bullets at once, default state just shoots one bullet.

Extension is different firing mode (three bullets at once).

Several testing configs added to test various cases.

+ and - keys can be used to change game speed in addition to the menu settings.