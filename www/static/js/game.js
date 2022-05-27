const WINDOW_HEIGHT = 960;
const WINDOW_WIDTH = 960;

const DEPTH = 2;
const MAP_SIZE = 40;

const SPRITE_WIDTH = WINDOW_WIDTH / MAP_SIZE;
const SPRITE_HEIGHT = WINDOW_HEIGHT / MAP_SIZE;

const ASSET_ROOT = "/static/img/assets/"

const NB_ROUNDS = 200;

const MAX_SPEED = 25;

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
};

let speed = 1;

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

    display(app) {
        app.stage.addChild(this);
    }
}

class Bread extends PIXI.Sprite {
    constructor(x, y) {
        super(textures.bread)
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

class Game {
    constructor() {
        this.app = new PIXI.Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
    }

    setupGame(stechecDump) {
        this.dump = stechecDump;
        this.papys = [];
        this.troupes = [];
        this.barriers = [];
        this.nests = [];
        this.bushes = [];
        this.setupMap();
        this.displayDucks(0);
        this.turn = 0;
        this.paused = false;
        this.action_index = false;
        this.frame = 0;
        this.bread = [];
    }

    setupMap() {
        const totalSize = MAP_SIZE * MAP_SIZE;
        const lowerMapString = this.dump[0].map.cells.substr(0, totalSize);
        const upperMapString = this.dump[0].map.cells.substr(totalSize, totalSize * 2);
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const sprite = createSprite(textures.grass[Math.floor(Math.random() * 3)], i, j);
                this.app.stage.addChild(sprite);
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                const symbol = upperMapString.charAt(char_index);
                switch (symbol) {
                    case '#':
                        const bush = new Bush(i, j);
                        bush.display(this.app);
                        this.bushes.push(bush);
                        break;
                    case 'S':
                        this.app.stage.addChild(createSprite(textures.spawn, i, j));
                        break;
                    case 'N':
                        const nest = new Nest(i, j, false, false);
                        nest.display(this.app);
                        this.nests.push(nest);
                        break;
                    case 'B':
                    case 'b':
                        const barrier = new Barrier(i, j, symbol === 'b');
                        barrier.display(this.app);
                        this.barriers.push(barrier);
                        break;
                    case 'X':
                        this.app.stage.addChild(createSprite(textures.hole, i, j));
                        break;
                    default:
                        if ('0' <= symbol && symbol <= '9') {
                            const papy = new Papy(i, j);
                            papy.display(this.app);
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
                        if (bush == null) {
                            bush = new Bush(i, j);
                            bush.display(this.app);
                            this.bushes.push(bush);
                        }
                        break;
                    case 'B':
                    case 'b':
                        //Custom logic
                        break;
                }
            }
        }

        this.clearBread();

        for (let pain of this.dump[index].map.pains) {
            const bread = new Bread(pain.pos.colonne, pain.pos.ligne);
            bread.display(this.app);
            this.bread.push(bread);
        }
    }


    clearBread() {
        for(let bread of this.bread) {
            this.app.stage.removeChild(bread);
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
                const duck = new Duck(troupe.maman.colonne, troupe.maman.ligne, troupe.dir);
                duck.display(this.app);
                this.troupes[k].push(duck);

                // Pushes the ducklings
                for (let l = 1; l < troupe.canards.length; l++) {
                    const canard = troupe.canards[l];
                    const duckling = new Duckling(canard.colonne, canard.ligne, this.computeDuckDirection(canard, troupe.canards[l-1]));
                    duckling.display(this.app);
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
                this.app.stage.removeChild(duck);
            }
        }
        this.troupes = [];
    }

    jumpToRound(index) {
        this.displayMapChanges(index);
        this.displayDucks(index);
    }

    async gameLoop(delta) {
        if (this.pause || this.turn >= 400)
            return;
        const actions = this.getTurnActions();
        if (actions.length === 0 || this.action_index >= actions.length) {
            this.displayMapChanges(this.turn);
            this.displayDucks(this.turn);
            this.turn += 1;
            this.action_index = 0;
            this.frame = 0;
            return;
        }
        const curr_action = actions[this.action_index];
        switch (curr_action.action_type) {
            case 'auto_move':
                this.avancer(this.frame, curr_action.player_id, curr_action.dir, curr_action.troupe_id);
                break;
            case 'avancer':
                this.avancer(this.frame, curr_action.player_id, curr_action.direction, curr_action.troupe_id);
                break;
        }
        this.frame += 1;

        if (this.frame >= animationDuration()) {
            this.frame = 0;
            this.action_index += 1;
        }
    }

    startReplay() {
        this.app.ticker.add(delta => this.gameLoop(delta));
    }

    getTurnActions() {
        const player_id = this.dump[this.turn].round.player_id;
        return this.dump[this.turn].players[player_id].last_actions.map(e => {
            return {...e, player_id: player_id}
        });
    }

    avancer(frame, player_id, dir, troupe_id) {
        const troupe = this.troupes[troupe_id + player_id * 2];
        /*
        if (frame === 0) {
        }*/

        for (let i = 1; i < troupe.length; i++) {
            const start_pos = [troupe[i].posX, troupe[i].posY];
            const end_pos = [troupe[i-1].posX, troupe[i-1].posY];
            end_pos[0] = calculateX(end_pos[0]);
            end_pos[1] = calculateY(end_pos[1]);
            start_pos[0] = calculateX(start_pos[0]);
            start_pos[1] = calculateY(start_pos[1]);
            troupe[i].x = ((1 + frame) * (end_pos[0] - start_pos[0]) / animationDuration() + start_pos[0]);
            troupe[i].y = ((1 + frame) * (end_pos[1] - start_pos[1]) / animationDuration() + start_pos[1]);
        }


        const start_pos = [troupe[0].posX, troupe[0].posY];
        const end_pos = this.calculateEndPosition(start_pos, dir);
        end_pos[0] = calculateX(end_pos[0]);
        end_pos[1] = calculateY(end_pos[1]);
        start_pos[0] = calculateX(start_pos[0]);
        start_pos[1] = calculateY(start_pos[1]);

        troupe[0].x = ((1 + frame) * (end_pos[0] - start_pos[0]) / animationDuration() + start_pos[0]);
        troupe[0].y = ((1 + frame) * (end_pos[1] - start_pos[1]) / animationDuration() + start_pos[1]);

        if (frame + 1 == animationDuration()) {
            troupe[0].posX = end_pos[0];
            troupe[0].posY = end_pos[1];
        }
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
            // TODO Handle the down and up case
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
                const char_index = i * MAP_SIZE + j;
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

function createSprite(texture, x, y) {
    let sprite = new PIXI.Sprite(texture);
    sprite.y = calculateY(y);
    sprite.x = calculateX(x);
    sprite.width = SPRITE_WIDTH;
    sprite.height = SPRITE_HEIGHT;
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

    display(app) {
        app.stage.addChild(this);
    }

    throwBread() {
        this.play();
    }
}

class Duck extends PIXI.AnimatedSprite {
    constructor(x, y, dir) {
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
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.dir = dir;
    }

    changeOrientation(dir) {
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
    constructor(x, y, dir) {
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
        this.width = SPRITE_WIDTH;
        this.height = SPRITE_HEIGHT;
        this.dir = dir;
    }

    changeOrientation(dir) {
        this.textures = this.spriteSheet[dir];
        this.dir = dir;
    }

    display(app) {
        app.stage.addChild(this);
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



