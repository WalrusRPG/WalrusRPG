By @Eiyeron.
This is only a paper where I'm collecting all the infos we need to work on.
Extra notes by Streetwalrus in (S: <insert random crap here>)

# Entity
- An entity must be placed between map's layer. (S: entity class should have a layer attribute)
- An entity should have differents behaviors according the the NPCs/enemies we want to do
  - AI : Composition? Derivation? @Eiyeron : I'd go for composition.
  - Scripts? (S: possibly lua or some other embeddable script language)
  -  An entity should be able to move around a map or stay static
    - AABB collision with map tiles.
- Velocity : pixels/seconds! (S: what ? movement code is completely internal to the entity)
- Movement target? Random? Patroll? This should be extendable the same way than the AI.
- Spritesheet animation.
- Controllable with events/scripts(?)
- Should we count levers, chests, objects as immovable entities?
  - It could make their creation easier but it also break the "Entity is movable or interactive abstract(?) character" idioms for a "dynamic interactive object". (S: not really, an entity is anything interactive/dynamic)

# Battle engine
- Checkout/move HBE to this once the overworld system is done
- Tie-in with script(?), inventory, experience, characters/enemies and events.

# Map
- Loading from file
- Dynamic tiles
  - Animated

# Events
- Scripting (?)
- Store a flag array to know where we should be in the timeline/sidequests
  - Check progression integrity and sequence-breaking-proof mesures/check?

# Textbox
- HBE does have logic for one but it's way too linked to its custom string parser because it allows the engine to trigger effects on string updating/drawing and not parsing.
  - So, split them and find out how to force the engine to trigger said effects in the middle of reading them?

# Items (inventory)
Disclaimer : Lots'o'text. Prepare your anus.
- How to store them? I have three general ideas/way to do:
  1. Golden Sun/Earthbound : one item (be it armor, consommable or key item) take one inventory slot
    - Pro : inventory becomes plain dumb to set
    - Con : have you ever thought that a nut would take as much space as a CLAYMORE?!
  2. Pokemon I/II/III gens : one kind of item takes one inventory slot
    - Pro : you possibly can have much more objects and be more prepared
    - Con : Also brings worse problems : a grinder will fill his bags with replinishing items and will seek his path through without having to relay upon inns. I had this problem on recent Pokémons generations once berries are in : one kind would replenish PP and thus, with acutal care, one could go for days of grinding without healing at a Center.
  3. Pokemon IV+ gens/Bravely Default : All item kinds are registered to the bag, one kind per slot and its only limits are about a kind's quantity, not the global quantity
    - Pro : DOn't care about storing/retrieving basic potions
    - Cons : The inventory data would be as big as the number of item kinds there is in a game. Be prepared to have a lot of data to manage. It also rapes reality much more than the 2 previous points because you have pretty much a freaking bottomless bag, or almost.
