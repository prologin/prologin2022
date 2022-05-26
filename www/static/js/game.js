const WINDOW_HEIGHT = 960;
const WINDOW_WIDTH = 960;

const DEPTH = 2;
const MAP_SIZE = 40;

const SPRITE_WIDTH = WINDOW_WIDTH / MAP_SIZE;
const SPRITE_HEIGHT = WINDOW_HEIGHT / MAP_SIZE;

const ASSET_ROOT = "/static/img/assets/"


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
};


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
                        const bush = createSprite(textures.bush, i, j);
                        this.bushes.push(bush);
                        this.app.stage.addChild(bush);
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


    jumpToRound(index) {
        //displayChanges(index);
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
    let texture = undefined;
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
                return createSprite(texture.old[0], i, j);
            }
            break;
    }
    if (texture !== undefined) {
        return createSprite(texture, i, j);
    }
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
    constructor(x, y) {
        const spriteSheet = {
            north: [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_N_3.png`)],
            south: [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_S_3.png`)],
        }
        super(spriteSheet['south'], true);
        this.spriteSheet = spriteSheet;
        this.animationSpeed = 0.1;
        this.loop = false;
        this.y = calculateY(y);
        this.x = calculateX(x);
    }

    display(app) {
        app.stage.addChild(this);
    }
}

class Duckling extends PIXI.AnimatedSprite {
    constructor(x, y) {
        const spriteSheet = {
            north: [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_N_3.png`)],
            south: [new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_1.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_2.png`),
                   new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duckling_S_3.png`)],
        }
        super(spriteSheet['south'], true);
        this.spriteSheet = spriteSheet;
        this.animationSpeed = 0.1;
        this.loop = false;
        this.y = calculateY(y);
        this.x = calculateX(x);
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



