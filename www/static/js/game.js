const WINDOW_HEIGHT = 960;
const WINDOW_WIDTH = 960;

const DEPTH = 2;
const MAP_SIZE = 40;

const SPRITE_WIDTH = WINDOW_WIDTH / MAP_SIZE;
const SPRITE_HEIGHT = WINDOW_HEIGHT / MAP_SIZE;

const ASSET_ROOT = "/static/img/assets/"

const NB_ROUNDS = 200;

const MAX_SPEED = 20;

const textures = {
    grass: [new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_1.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_2.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_3.png`)],
    old: [new PIXI.Texture.from(`${ASSET_ROOT}/old/old_1.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/old/old_2.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/old/old_3.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/old/old_4.png`),
        new PIXI.Texture.from(`${ASSET_ROOT}/old/old_1.png`),],
    pigeon: new PIXI.Texture.from(`${ASSET_ROOT}/pigeon/pigeon.png`),
    dirt: new PIXI.Texture.from(`${ASSET_ROOT}/dirt/dirt.png`),
    spawn: new PIXI.Texture.from(`${ASSET_ROOT}/spawn.png`),
    nest_empty: new PIXI.Texture.from(`${ASSET_ROOT}/nest_empty.png`),
    nest_full: new PIXI.Texture.from(`${ASSET_ROOT}/nest_full.png`),
    bush: new PIXI.Texture.from(`${ASSET_ROOT}/buisson.png`),
    barrier_closed: new PIXI.Texture.from(`${ASSET_ROOT}/barrier/barrier_1.png`),
    barrier_open: new PIXI.Texture.from(`${ASSET_ROOT}/barrier/gate.png`),
    hole: new PIXI.Texture.from(`${ASSET_ROOT}/trou.png`),
    bread: new PIXI.Texture.from(`${ASSET_ROOT}/bread.png`),
    rock: new PIXI.Texture.from(`${ASSET_ROOT}/rock.png`),
};

let speed = 10;

let ticker = PIXI.Ticker.shared;

function get_ticker() {
    return ticker;
}

function animationDuration() {
    return 1 + MAX_SPEED - speed;
}


function generateMatrix() {
    let list = [];
    for (let i = 0; i < MAP_SIZE; i++) {
        list.push([]);
        for (let j = 0; j < MAP_SIZE; j++) {
            list[i].push([]);
        }
    }
    return list;
}

function create_game() {
    return new Game();
}

function calculateY(y) {
    return WINDOW_HEIGHT - y * SPRITE_HEIGHT - SPRITE_HEIGHT;
}

function calculateX(x) {
   return x * SPRITE_WIDTH;
}

function colorToHTML(color) {
    switch (color) {
        case 1:
            return 0x3333FF;
        case 2:
            return 0xFFFF00;
        case 3:
            return 0xFF9999;
    }
}


class Barrier extends PIXI.Sprite {
    constructor(x, y, isOpen) {
        super(isOpen ? textures.barrier_open : textures.barrier_closed);
        this.posX = x;
        this.posY = y
        this.y = calculateY(y);
        this.x = calculateX(x);
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.isOpen = isOpen;
    }

    display(app) {
        app.stage.addChild(this);
    }

    swap() {
        if (this.isOpen) {
            this.texture = textures.barrier_closed;
        } else {
            this.texture = textures.barrier_open;
        }
        this.isOpen = !this.isOpen;
    }
}

class Nest extends PIXI.Sprite {
    constructor(x, y, isTaken, hasEggs) {
        super(hasEggs ? textures.nest_full : textures.nest_empty);
        this.posX = x;
        this.posY = y;
        this.y = calculateY(y);
        this.x = calculateX(x);
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
    }
}

class Bread extends PIXI.Sprite {
    constructor(x, y, z) {
        super(textures.bread)
        this.x = calculateX(x);
        this.y = calculateY(y);
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.posX = x;
        this.posY = y;
        this.posZ = z;
    }

    display(app) {
        app.stage.addChild(this);
    }
}

class DisplayManager {

    constructor(app) {
        this.app = app;
        this.topSprites = [];
        this.bottomSprites = [];
        this.showBottom = false;
    }

    addTopSprite(ent) {
        this.topSprites.push(ent);
        if (!this.showBottom)
            this.app.stage.addChild(ent);
    }

    addBottomSprite(ent) {
        this.bottomSprites.push(ent);
        if (this.showBottom)
            this.app.stage.addChild(ent);
    }

    addSprite(ent, level) {
        if (level === 0)
            this.addTopSprite(ent);
        else
            this.addBottomSprite(ent);
    }

    rerender(sprite) {
        if (this.showBottom) {
            for (const sprite of this.bottomSprites) {
                this.app.stage.removeChild(sprite);
                this.app.stage.addChild(sprite);
                return;
            }
        }
        for (const sprite of this.topSprites) {
            this.app.stage.addChild(sprite);
            this.app.stage.addChild(sprite);
            return;
        }
    }

    goUp(sprite) {
        for (let i = 0; i < this.bottomSprites.length; i++) {
            const val = this.bottomSprites[i];
            if (val === sprite) {
                this.bottomSprites.splice(i, 1);
                this.app.stage.removeChild(sprite);
                this.addTopSprite(val);
                return;
            }
        }
    }


    goDown(sprite) {
        for (let i = 0; i < this.topSprites.length; i++) {
            const val = this.topSprites[i];
            if (val === sprite) {
                this.topSprites.splice(i, 1);
                this.app.stage.removeChild(sprite);
                this.addBottomSprite(val);
                return;
            }
        }
    }

    displayTop() {
        if (this.showBottom) {
            for (const sprite of this.bottomSprites) {
                this.app.stage.removeChild(sprite);
            }
        }
        for (const sprite of this.topSprites) {
            this.app.stage.addChild(sprite);
        }
        this.showBottom = false;
    }

    displayBottom() {
        if (this.showBottom) {
            for (const sprite of this.topSprites) {
                this.app.stage.removeChild(sprite);
            }
        }
        for (const sprite of this.bottomSprites) {
            this.app.stage.addChild(sprite);
        }
        this.showBottom = true;
    }

    changeLevel() {
        if (this.showBottom) {
            this.displayTop();
        } else {
            this.displayBottom();
        }
    }

    removeSprite(input) {
        for (let i = 0; i < this.topSprites.length; i++) {
            if (this.topSprites[i] === input){
                this.app.stage.removeChild(this.topSprites[i]);
                this.topSprites.splice(i, 1);
                return;
            }
        }
        for (let i = 0; i < this.bottomSprites.length; i++) {
            if (this.bottomSprites[i] === input){
                this.app.stage.removeChild(this.bottomSprites[i]);
                this.bottomSprites.splice(i, 1);
                return;
            }
        }
    }
}

class Game {
    constructor() {
        this.app = new PIXI.Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
        this.displayManager = new DisplayManager(this.app);
    }

    setupGame(stechecDump) {
        this.dump = stechecDump;
        this.papys = [];
        this.troupes = [[], [], [], []];
        this.barriers = [];
        this.nests = [];
        this.bushes = [];
        this.rocks = [];
        this.setupMap();
        this.displayDucks(0);
        this.turn = 1;
        this.paused = true;
        this.action_index = 0;
        this.frame = 0;
        this.bread = [];
        this.pigeons = [];
    }

    bindSlider(slider) {
        this.slider = slider;
    }

    updateSlider(index) {
        this.slider.val(index).trigger('change');
    }


    setupMap() {
        const totalSize = MAP_SIZE * MAP_SIZE;
        const lowerMapString = this.dump[0].map.cells.substr(0, totalSize);
        const upperMapString = this.dump[0].map.cells.substr(totalSize, totalSize * 2);
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const sprite = createSprite(textures.grass[Math.floor(Math.random() * 3)], i, j);
                this.displayManager.addTopSprite(sprite);
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const dirt = createSprite(textures.dirt, i, j);
                const rock = new Rock(i, j);
                this.displayManager.addBottomSprite(dirt);
                this.displayManager.addBottomSprite(rock);
                this.rocks.push(rock);
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                const symbol = upperMapString.charAt(char_index);
                switch (symbol) {
                    case '#':
                        const bush = new Bush(i, j);
                        this.displayManager.addTopSprite(bush);
                        this.bushes.push(bush);
                        break;
                    case 'S':
                        this.displayManager.addTopSprite(createSprite(textures.spawn, i, j));
                        break;
                    case 'N':
                        const nest = new Nest(i, j, false, false);
                        this.displayManager.addTopSprite(nest);
                        this.nests.push(nest);
                        break;
                    case 'B':
                    case 'b':
                        const barrier = new Barrier(i, j, symbol === 'b');
                        this.displayManager.addTopSprite(barrier);
                        this.barriers.push(barrier);
                        break;
                    case 'X':
                        this.displayManager.addTopSprite(createSprite(textures.hole, i, j));
                        break;
                    default:
                        if ('0' <= symbol && symbol <= '9') {
                            const papy = new Papy(i, j);
                            this.displayManager.addTopSprite(papy);
                            this.papys.push(papy);
                            break;
                        }
                        break;
                }
            }
        }

    }

    displayMapChanges(index) {
        const totalSize = MAP_SIZE * MAP_SIZE;
        const lowerMapString = this.dump[index].map.cells.substr(0, totalSize);
        const upperMapString = this.dump[index].map.cells.substr(totalSize, totalSize * 2);
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                const symbol = upperMapString.charAt(char_index);
                switch (symbol) {
                    case '#':
                        let bush = this.findBushByCoords(i, j);
                        if (bush === null) {
                            bush = new Bush(i, j);
                            this.displayManager.addTopSprite(bush);
                            this.bushes.push(bush);
                        }
                        break;
                    case 'B':
                        for (let barrier of this.barriers) {
                            if (barrier.getX === i && barrier.getY === j) {
                                if (!barrier.isOpen) {
                                    barrier.swap();
                                }
                            }
                        }
                        break;
                    case 'b':
                        for (let barrier of this.barriers) {
                            if (barrier.getX === i && barrier.getY === j) {
                                if (barrier.isOpen) {
                                    barrier.swap();
                                }
                            }
                        }
                        break;
                }
            }
        }

        this.clearRocks();

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                const symbol = lowerMapString.charAt(char_index);
                switch (symbol) {
                    case 't':
                        break;
                    case 'T':
                        const rock = new Rock(i, j);
                        this.displayManager.addBottomSprite(rock);
                        this.rocks.push(rock);
                        break;
                }
            }
        }

        this.clearBread();

        for (let pain of this.dump[index].map.pains) {
            const bread = new Bread(pain.pos.colonne, pain.pos.ligne);
            this.displayManager.addSprite(bread, pain.pos.niveau);
            this.bread.push(bread);
        }
    }

    clearRocks() {
        for (let rock of this.rocks) {
            this.displayManager.removeSprite(rock);
        }
        this.rocks = [];
    }


    clearBread() {
        for(let bread of this.bread) {
            this.displayManager.removeSprite(bread);
        }
        this.bread = [];
    }

    findBushByCoords(x, y) {
        for (const bush of this.bushes) {
            if (bush.posX === x && bush.posY === y) {
                return bush;
            }
        }
        return null;
    }

    displayDucks(index) {
        this.clearDucks();
        const roundData = this.dump[index].players;
        for (let i = 0, k = 0; i < roundData.length; i++) {
            for (let j = 0; j < roundData[i].troupes.length; j++, k++) {
                this.troupes[k] = [];
                // Display the duck.
                const troupe = roundData[i].troupes[j];
                const duck = new Duck(troupe.maman.colonne, troupe.maman.ligne, troupe.maman.niveau, troupe.dir, i);
                this.displayManager.addSprite(duck, duck.posZ);
                this.troupes[k].push(duck);

                // Pushes the ducklings
                for (let l = 1; l < troupe.canards.length; l++) {
                    const canard = troupe.canards[l];
                    const duckling = new Duckling(canard.colonne, canard.ligne, canard.niveau, this.computeDuckDirection(canard, troupe.canards[l-1]), i);
                    this.displayManager.addSprite(duckling, duckling.posZ);
                    this.troupes[k].push(duckling);
                }
            }
        }
    }

    computeDuckDirection(duck, next) {
        if (next.colonne > duck.colonne) {
            return 2;
        } else if (next.colonne < duck.colonne) {
            return 3;
        } else if (next.ligne > duck.ligne) {
            return 0;
        } else {
            return 1;
        }
        return 1;
    }

    clearDucks() {
        for (const troupe of this.troupes) {
            for (const duck of troupe) {
                this.displayManager.removeSprite(duck);
            }
        }
        this.troupes = [];
    }

    clearPigeons() {
        for (const pigeon of this.pigeons) {
            this.displayManager.removeSprite(pigeon);
        }
        this.pigeons = [];
    }

    jumpToRound(index) {
        this.turn = index + 1;
        this.frame = 0;
        this.action_index = 0;
        this.displayMapChanges(index);
        this.displayDucks(index);
        this.clearPigeons();
    }

    pauseGame() {
        this.paused = !this.paused;
    }

    gameLoop(delta, useSlider) {
        if (this.paused || this.turn > 400)
            return;

        const actions = this.getTurnActions();

        const curr_action = actions[this.action_index];

        if (curr_action === undefined) {
            this.frame = 0;
            this.action_index = 0;
            this.clearPigeons();
            this.turn += 1;
            if (useSlider)
                this.updateSlider(this.turn);
            return;
        }

        switch (curr_action.action_type) {
            case 'construire':
                this.construire(this.frame, curr_action.position);
                this.frame = animationDuration();
                break;
            case 'creuser':
                this.creuser(this.frame, curr_action.position);
                this.frame = animationDuration();
                break;
            case 'auto_move':
                this.avancer(this.frame, curr_action.player_id, curr_action.dir, curr_action.troupe_id);
                break;
            case 'avancer':
                this.avancer(this.frame, curr_action.player_id, curr_action.direction, curr_action.troupe_id);
                break;
            case 'capture_nest':
                this.captureNest(this.frame, curr_action.player_id, curr_action.pos);
                this.frame = animationDuration();
                break;
            case 'debug':
                this.createDebugPigeon(curr_action.player_id, curr_action.pos, curr_action.debug);
                this.frame = animationDuration();
                break;
            case 'respawn':
                this.respawn(this.frame, curr_action.player_id, curr_action.troupe_id, curr_action.pos);
                this.frame = animationDuration();
                break;
            case 'nouveau_canard':
                this.new_duck(this.frame, curr_action.player_id, curr_action.troupe_id, curr_action.pos);
                this.frame = animationDuration();
                break;
            case 'spread_bread':
            case 'add_bread':
                this.addBread(this.frame, curr_action.pos, curr_action.player_id);
                this.frame = animationDuration();
                break;
            case 'take_bread':
                this.takeBread(this.frame, curr_action.pos, curr_action.player_id);
                this.frame = animationDuration();
                break;
        }

        this.frame += 1;
        if (this.frame >= animationDuration()) {
            this.frame = 0;
            this.action_index += 1;
        }
    }

    creuser(frame, position) {
        for (let i = 0; i < this.rocks.length; i++) {
            const rock = this.rocks[i];
            if (rock.posX === position.colonne && rock.posY === position.ligne) {
                this.rocks.splice(i, 1);
                this.displayManager.removeSprite(rock);
                return;
            }
        }
    }

    construire(frame, position) {
        let bush = new Bush(position.colonne, position.ligne);
        this.displayManager.addTopSprite(bush);
        this.bushes.push(bush);
    }

    createDebugPigeon(player_id, pos, debug) {
        for (let i = 0; i < this.pigeons.length ; i++) {
            if (pos.colonne == this.pigeons[i].posX &&
                pos.ligne == this.pigeons[i].posY && pos.niveau == this.pigeons[i].posZ) {
                if (debug === 0) {
                    this.displayManager.removeSprite(this.pigeons[i]);
                    this.pigeons[i].remove(i);
                } else {
                    this.pigeons[i].tint = colorToHTML(debug);
                }
                return;
            }
        }
        const pigeon = createSprite(textures.pigeon, pos.colonne, pos.ligne);
        pigeon.posX = pos.colonne;
        pigeon.posY = pos.ligne;
        pigeon.posZ = pos.niveau;
        pigeon.tint = colorToHTML(debug);
        this.displayManager.addSprite(pigeon, pos.niveau);
        this.pigeons.push(pigeon);
    }

    captureNest(frame, player_id, pos) {
        for (let nest of this.nests) {
            if (nest.posX === pos.colonne && nest.posY === pos.ligne) {
                if (player_id === 0) {
                    nest.tint = 0x990000;
                } else {
                    nest.tint = 0x3333FF;
                }
            }
        }
    }

    addBread(frame, pos, player_id) {
        for (let papy of this.papys) {
            if (papy.posX === pos.colonne && papy.posY === pos.ligne && pos.niveau === 0) {
                papy.throwBread();
                const bread = new Bread(pos.colonne, pos.ligne);
                this.bread.push(bread);
                this.displayManager.addSprite(bread, pos.niveau);
                this.displayManager.rerender(papy);
                return;
            }
        }
        const bread = new Bread(pos.colonne, pos.ligne, pos.niveau);
        this.displayManager.addSprite(bread, pos.niveau);
        this.bread.push(bread);
    }

    takeBread(frame, pos, player_id) {
        for (let bread of this.bread) {
            if (bread.posX === pos.colonne && bread.posY === pos.ligne && bread.posZ === pos.niveau) {
                this.displayManager.removeSprite(bread);
            }
        }
    }

    respawn(frame, player_id, troupe_id, pos) {
        if (this.troupes[troupe_id - 1 + 2 * player_id]) {
            for(let troupe of this.troupes[troupe_id - 1 + 2 * player_id]) {
                this.displayManager.removeSprite(troupe);
            }
        }
        this.troupes[troupe_id - 1 + 2 * player_id] = [new Duck(pos.colonne, pos.ligne, pos.niveau, 1, player_id)];
        const sprite = this.troupes[troupe_id - 1 + 2 * player_id][0];
        this.displayManager.addSprite(sprite, sprite.posZ);
    }


    new_duck(frame, player_id, troupe_id, pos) {
        const index = (troupe_id - 1) + 2 * player_id;
        //TODO Direction
        this.troupes[index].push(new Duckling(pos.colonne, pos.ligne, pos.niveau, 1, player_id));
        const sprite = this.troupes[index][this.troupes[index].length - 1];
        this.displayManager.addSprite(sprite, sprite.posZ);
    }

    async stream() {
        this.paused = false;
        ticker.start();
        while(this.turn <= 400) {
            await new Promise(r => setTimeout(r, 20000));
        }
        console.log('hey')
    }

    async startReplay() {
        this.app.ticker.add(this.gameLoop, this);
    }

    getTurnActions() {
        const player_id = this.dump[this.turn].round.player_id;
        return this.dump[this.turn].players[player_id].last_actions.map(e => {
            return {...e, player_id: player_id}
        });
    }

    avancer(frame, player_id, dir, troupe_id) {
        const troupe = this.troupes[(troupe_id - 1) + player_id * 2];
        if (frame === 0) {
            troupe[0].changeOrientation(dir);
        }
        if (dir == 5) {
            this.displayManager.goDown(troupe[0]);
        } else if (dir == 5) {
            this.displayManager.goUp(troupe[0]);
        }
        for (let i = troupe.length - 1; i >= 1; i--) {
            if (frame === 0) {
                troupe[i].changeOrientation(troupe[i-1].dir);
            }
            const startpos = [calculateX(troupe[i].posX), calculateY(troupe[i].posY)];
            const endpos = [calculateX(troupe[i-1].posX), calculateY(troupe[i-1].posY)];

            if (dir == 5) {
                this.displayManager.goDown(troupe[0]);
            } else if (dir == 4) {
                this.displayManager.goUp(troupe[0]);
            }


            troupe[i].x = ((1 + frame) * (endpos[0] - startpos[0]) / animationDuration() + startpos[0]);
            troupe[i].y = ((1 + frame) * (endpos[1] - startpos[1]) / animationDuration() + startpos[1]);
            if (frame + 1 === animationDuration()) {
                troupe[i].posX = troupe[i-1].posX;
                troupe[i].posY = troupe[i-1].posY;
            }

            troupe[i].width = SPRITE_WIDTH;
            troupe[i].height = SPRITE_HEIGHT;
            troupe[i].gotoAndPlay(frame % 3);
            troupe[i].width = SPRITE_WIDTH;
            troupe[i].height = SPRITE_HEIGHT;

        }

        const endposN = this.calculateEndPosition([troupe[0].posX, troupe[0].posY], dir);

        const startpos = [calculateX(troupe[0].posX), calculateY(troupe[0].posY)];
        const endpos = [calculateX(endposN[0]), calculateY(endposN[1])];

        troupe[0].x = ((1 + frame) * (endpos[0] - startpos[0]) / animationDuration() + startpos[0]);
        troupe[0].y = ((1 + frame) * (endpos[1] - startpos[1]) / animationDuration() + startpos[1]);
        if (frame + 1 === animationDuration()) {
            troupe[0].posX = endposN[0];
            troupe[0].posY = endposN[1];
        }
        troupe[0].width = SPRITE_WIDTH;
        troupe[0].height = SPRITE_HEIGHT;
        troupe[0].gotoAndPlay(frame % 3);
        troupe[0].width = SPRITE_WIDTH;
        troupe[0].height = SPRITE_HEIGHT;
    }

    calculateEndPosition(start, dir) {
        switch (dir) {
            case 0:
                return [start[0], start[1] + 1];
            case 1:
                return [start[0], start[1] - 1];
            case 2:
                return [start[0] + 1, start[1]];
            case 3:
                return [start[0] - 1, start[1]];
            case 4:
                return start;
            case 5:
                return start;
        }
    }


    displaySimpleMap(mapString) {
        // Default map with texturing to allow for transparency.
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const sprite = createSprite(textures.grass[Math.floor(Math.random() * 3)], i, j);
                this.app.stage.addChild(sprite);
            }
        }
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = j * MAP_SIZE + i;
                const sprite = map_char_to_sprite(mapString.charAt(char_index), i, j);
                this.app.stage.addChild(sprite);
            }
        }
    }


    addToDOM(viewParent) {
        return viewParent.appendChild(this.app.view);
    }
}

function createGame() {
    return new Game();
}


class Rock extends PIXI.Sprite {

    constructor(x, y) {
        super(textures.rock)
        this.x = calculateX(x);
        this.y = calculateY(y);
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.posX = x;
        this.posY = y;
        this.posZ = -1;
    }
}

function createSprite(texture, x, y) {
    let sprite = new PIXI.Sprite(texture);
    sprite.y = calculateY(y);
    sprite.x = calculateX(x);
    sprite.width = SPRITE_WIDTH;
    sprite.height = SPRITE_HEIGHT;
    this.posX = x;
    this.posY = y;
    return sprite;
}

function map_char_to_sprite(input_char, i, j) {
    let texture = textures.grass[0];
    switch (input_char) {
        case ' ':
        case '.':
            texture = textures.grass[Math.floor(Math.random() * 3)];
            break;
        case 'S':
            texture = textures.spawn;
            break;
        case 'N':
            texture = textures.nest_empty;
            break;
        case '#':
            texture = textures.bush;
            break;
        case 'B':
            texture = textures.barrier_closed;
            break;
        case 'b':
            texture = textures.barrier_open;
            break;
        case 'X':
            texture = textures.hole;
            break;
        default:
            if ('0' <= input_char && input_char <= '9') {
                return createSprite(textures.old[0], i, j);
            }
    }
    return createSprite(texture, i, j);
}

class Papy extends PIXI.AnimatedSprite {
    constructor(x, y) {
        super(textures.old, true);
        this.animationSpeed = 0.1;
        this.loop = false;
        this.posX = x;
        this.posY = y;
        this.y = calculateY(y);
        this.x = calculateX(x);
    }

    throwBread() {
        this.play();
    }
}

class Duck extends PIXI.AnimatedSprite {
    constructor(x, y, z, dir, player_id) {
        const spriteSheet = [
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_E_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_E_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_E_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_W_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_W_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_W_3.png`)],
        ]
        super(spriteSheet[dir], true);
        this.spriteSheet = spriteSheet;
        this.animationSpeed = 0.1;
        this.loop = false;
        this.y = calculateY(y);
        this.x = calculateX(x);
        this.posX = x;
        this.posY = y;
        this.posZ = z;
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.dir = dir;
        if (player_id === 1)
            this.tint = 0xFF00FF;
    }

    changeOrientation(dir) {
        if (dir == 4 || dir == 5)
            return;
        this.textures = this.spriteSheet[dir];
        this.dir = dir;
    }

    display(app) {
        app.stage.addChild(this);
    }
}

class Bush extends PIXI.Sprite {
    constructor(x, y) {
        super(textures.bush);
        this.x = calculateX(x);
        this.y = calculateY(y);
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.posX = x;
        this.posY = y;
    }

    display(app) {
        app.stage.addChild(this);
    }
}

class Duckling extends PIXI.AnimatedSprite {
    constructor(x, y, z, dir, player_id) {
        const spriteSheet = [
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_E_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_E_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_E_3.png`)],
            [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_W_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_W_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_W_3.png`)],
        ];
        super(spriteSheet[dir], true);
        this.spriteSheet = spriteSheet;
        this.animationSpeed = 0.1;
        this.loop = false;
        this.y = calculateY(y);
        this.x = calculateX(x);
        this.posX = x;
        this.posY = y;
        this.posZ = z;
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.dir = dir;
        if (player_id === 1)
            this.tint = 0xFF00FF;
    }

    changeOrientation(dir) {
        this.textures = this.spriteSheet[dir];
        this.dir = dir;
    }
}

function showDuckTooltip(player, index, x, y) {
    let container = new PIXI.Container();
    const size = 200;
    const round = 10;

    var rectangle = new PIXI.Graphics();
    rectangle.beginFill(0xC0C0C0);
    rectangle.drawRoundedRect(0, 0, size, size, round);
    rectangle.width = size;
    rectangle.height = size;

    const title = new PIXI.Text('Troupe ' + player.troupes[index].id + ' (' + player.name + ')',
                                {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 14, fill : 0x000000});
    title.x = 2 * round;
    title.y = round;

    const troupe_size = new PIXI.Text('Taille : ' + player.troupes[index].taile,
                                {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
    troupe_size.y = title.y + 2 * 14;
    troupe_size.x = title.x;


    const inventory = new PIXI.Text('Inventaire : ' + player.troupes[index].inventaire,
                                {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
    inventory.y = troupe_size.y + 2 * 14;
    inventory.x = title.x;

    const actions_points = new PIXI.Text('Points d\'action : ' + player.troupes[index].pts_actions,
                                {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
    actions_points.y = inventory.y + 2 * 14;
    actions_points.x = title.x;

    container.y = x * SPRITE_WIDTH + size > WINDOW_HEIGHT ? WINDOW_HEIGHT - size - round: x * SPRITE_WIDTH;
    container.x = y * SPRITE_WIDTH + size > WINDOW_HEIGHT ? WINDOW_HEIGHT - size - round: y * SPRITE_WIDTH;
    container.interactive = true;

    container.on('click', (e) => {
        e.stopPropagation();
        this.app.stage.removeChild(container);
    })

    container.addChild(rectangle);
    container.addChild(title);
    container.addChild(troupe_size);
    container.addChild(inventory);
    container.addChild(actions_points);

    this.app.stage.addChild(container);
}



