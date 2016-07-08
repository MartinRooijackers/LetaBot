

My first attempt at atuomatic combat heuristic generator.

The original idea was to go for genetic programming. 
But with only 1 computer available, I was better of using a more simple method.
Even then, running this for 5 days (10 hours each day) doesn't get close to the kind of heuristics that I need.


Anyway, 

Things the bot did learn:

-Hit & Run marine micro. But instead of move it uses the patrol command.
I still have to check if that really makes a difference. 
Maybe the patrol causes the unit to turn as soon as the weapon cooldown is 0.

- Attack_closest commands when zerlings (melee units) are close,
then attack_lowest_hp when fighting against the remaining hydralisks (ranged units).

- Burrowing when marines are close.
I also think I saw something like hold lurkers (wating till the enemy is close to attack).
But instead of the "Hold Position" command (which is not included in the list of commands the bot can use),
it spams a non-combat action (I guess it was a move_direction). 
Just like human players can spam the stop command to prevent the lurker from attacking.

- Medics using attack_move to heal ( known as a heal move)

- Valkyries learning to prioritise scourges over mutalisks

- Dark Swarm and Ensnare combo


--------------------------------------------------------------------------------------------



This prject is open source, so if you like what you see above, feel free to run the DLL yourself.
There are still a lot of TODO's in the code though. For now it will only TvZ.


Put the "Micro" folder in the bwapi-data\write folder.
It already containts some heuristics. If you want to start anew, 
trow the folders 0 till 249 away and run the GenerareDir.py python script



