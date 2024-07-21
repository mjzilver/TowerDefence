import { Monster } from './monster.js';

export class Boss extends Monster {
    constructor(x, y) {
        super(x, y);
        this.char = 'B';
        this.path = [];
        this.hp = 100;
        this.damage = 10;
    }

    moveTo(x, y) {
        this.x = x;
        this.y = y;
    }

    attack(building) {
        building.hp -= this.damage;
    }

    setPath(path) {
        this.path = path;
    }

    getPath() {
        return this.path;
    }
}