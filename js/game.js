import { Base } from './entities/base.js';
import { Tower } from './entities/tower.js';
import { Wall } from './entities/wall.js';
import { Particle } from './entities/particle.js';
import { Bomb } from './entities/bomb.js';
import { BombTower } from './entities/bomb-tower.js';

import { render } from './renderer.js';
import { outOfBoundsCheck } from './utils.js';
import { MonsterSpawner } from './monster-spawner.js';
import { drawUI } from './ui.js';
import { Quadtree, Rectangle } from './quadtree.js';
import { PathFinder } from './pathfinding.js';

class Game {
    constructor() {
        this.canvas = document.getElementById('gameCanvas');
        this.ctx = this.canvas.getContext('2d');

        this.canvas.width = Math.max(window.innerWidth, 900);
        this.canvas.height = Math.max(window.innerHeight, 900);

        this.tileSize = 24;
        this.mapWidth = 30;
        this.mapHeight = 30;

        this.FPS_MONSTERS = 2;
        this.FPS_ARROWS = 60;
        this.FRAME_DURATION_MONSTERS = 1000 / this.FPS_MONSTERS;
        this.FRAME_DURATION = 1000 / this.FPS_ARROWS;
        this.lastUpdateTimeMonsters = 0;
        this.lastUpdateTimeArrows = 0;

        this.isGameOver = false;
        this.gameTicks = 0;
        this.gold = 100;
        this.level = 1;
        this.cameraX = 0;
        this.cameraY = 0;

        this.monsterSpawner = new MonsterSpawner(this.mapWidth, this.mapHeight);
        this.quadtree = new Quadtree(0, 0, this.mapWidth * this.tileSize, this.mapHeight * this.tileSize);
        this.pathFinder = new PathFinder(this.mapWidth, this.mapHeight, this);

        this.entities = {
            base: new Base(this.mapWidth / 2, this.mapHeight / 2),
            towers: [],
            walls: [],
            monsters: [],
            arrows: [],
            particles: []
        };

        this.initEventListeners();
        this.gameLoop();
    }

    initEventListeners() {
        window.addEventListener('click', (event) => this.handleClick(event));
        window.addEventListener('keydown', (event) => this.handleKeydown(event));
    }

    gameLoop() {
        const currentTime = Date.now();
        const deltaTimeMonsters = currentTime - this.lastUpdateTimeMonsters;

        if (deltaTimeMonsters >= this.FRAME_DURATION_MONSTERS && !this.isGameOver) {
            this.updateMonsters();
            this.lastUpdateTimeMonsters = currentTime;
        }

        const deltaTime = currentTime - this.lastUpdateTimeArrows;
        if (deltaTime >= this.FRAME_DURATION && !this.isGameOver) {
            this.updateArrows();
            this.updateParticles();
            this.monsterSpawner.spawn(this.entities.monsters, this.gameTicks, this);
            this.lastUpdateTimeArrows = currentTime;
            this.gameTicks++;

            this.checkCollisions();
        }

        if (this.isGameOver && this.entities.particles.length !== 0) {
            this.gameTicks++;
            this.updateParticles();
        }

        render(this.ctx, this.entities, this.cameraX, this.cameraY, this.tileSize, this.mapWidth, this.mapHeight);

        drawUI(this.ctx, this, this.mapWidth * this.tileSize, this.mapHeight * this.tileSize);

        // logs how many ticks behind the game is
        let ticksBehind = Math.floor((Date.now() - currentTime) / this.FRAME_DURATION);
        if (ticksBehind > 15) {
            console.log(`Ticks behind: ${ticksBehind}`);
        }

        requestAnimationFrame(() => this.gameLoop());
    }

    combineMonsters(monster, monsterAt) {
        monster.hp += monsterAt.hp;
        monster.damage += monsterAt.damage;
        monster.startHp += monsterAt.startHp;
        monster.char = monsterAt.char;
        this.entities.monsters = this.entities.monsters.filter(m => m !== monsterAt);
    }

    invalidateAllPaths() {
        this.entities.monsters.forEach(monster => {
            monster.setPath([]);
        });
    }

    updateMonsters() {
        this.entities.monsters.forEach(monster => {
            let path = monster.getPath();

            // check if the saved path is still good
            if (!path || path.length <= 1) {
                path = this.pathFinder.findPath(monster.x, monster.y, this.entities.base.x, this.entities.base.y, monster);
                monster.setPath(path);
            } else {
                path.shift();
                monster.setPath(path);
            }

            if (path.length > 1) {
                const nextPosition = path[1];
                let building = this.getBuildingAtPosition(nextPosition.x, nextPosition.y);
                let monsterAt = this.entities.monsters.find(m => m.x === nextPosition.x && m.y === nextPosition.y && m !== monster);

                if (monsterAt) {
                    this.combineMonsters(monster, monsterAt);
                }

                if (building) {
                    monster.attack(building);
                    if (building.hp <= 0) {
                        this.destroyBuilding(building);
                    }
                } else {
                    monster.moveTo(nextPosition.x, nextPosition.y);
                }
            }
        });
    }

    updateArrows() {
        this.entities.towers.forEach(tower => {
            if (!tower.canShoot(this.gameTicks)) return;

            const monster = this.entities.monsters.find(monster => {
                return Math.abs(monster.x - tower.x) <= tower.range && Math.abs(monster.y - tower.y) <= tower.range;
            });

            if (monster) {
                const monsterPath = monster.getPath();
                const stepsAhead = 2;
                let monsterNextPosition = { x: monster.x, y: monster.y };
                if (monsterPath.length > stepsAhead) {
                    monsterNextPosition = monsterPath[stepsAhead];
                }

                const dx = monsterNextPosition.x - tower.x;
                const dy = monsterNextPosition.y - tower.y;
                const length = Math.sqrt(dx * dx + dy * dy);
                const speed = this.tileSize / 8;

                const vx = (dx / length) * speed;
                const vy = (dy / length) * speed;

                const towerMiddleX = (tower.x * this.tileSize);
                const towerMiddleY = (tower.y * this.tileSize) + this.tileSize;

                let arr = tower.createArrow(towerMiddleX, towerMiddleY, vx, vy);

                this.entities.arrows.push(arr);

                tower.lastShot = this.gameTicks;
            }
        });

        this.entities.arrows.forEach(arrow => {
            arrow.move();

            if (outOfBoundsCheck(arrow.x, arrow.y, this.mapWidth * this.tileSize, this.mapHeight * this.tileSize)) {
                this.entities.arrows = this.entities.arrows.filter(a => a !== arrow);
            }
        });
    }

    updateParticles() {
        this.entities.particles.forEach(particle => {
            particle.move();

            if (!particle.isAlive(this.gameTicks)) {
                this.entities.particles = this.entities.particles.filter(p => p !== particle);
            }
        });
    }

    getBuildingAtPosition(x, y) {
        if (this.entities.base.x === x && this.entities.base.y === y) {
            return this.entities.base;
        }

        const wall = this.entities.walls.find(wall => wall.x === x && wall.y === y);
        if (wall) {
            return wall;
        }

        const tower = this.entities.towers.find(tower => tower.x === x && tower.y === y);
        if (tower) {
            return tower;
        }

        return null;
    }

    isPositionOccupied(x, y) {
        return this.getBuildingAtPosition(x, y) !== null || this.entities.monsters.some(monster => monster.x === x && monster.y === y) || outOfBoundsCheck(x, y, this.mapWidth, this.mapHeight);
    }

    destroyBuilding(building) {
        if (building instanceof Wall) {
            this.entities.walls = this.entities.walls.filter(wall => wall !== building);
            this.createParticlesAtTile(20, building.x, building.y, "black");
        } else if (building instanceof Tower) {
            this.entities.towers = this.entities.towers.filter(tower => tower !== building);
            this.createParticlesAtTile(30, building.x, building.y, "black");
        } else if (building instanceof Base) {
            this.createParticlesAtTile(500, building.x, building.y, "black", 300);
            this.isGameOver = true;
        }
    }

    createParticlesAtTile(amount, tileX, tileY, color = "red", aliveForTicks = 15) {
        const x = tileX * this.tileSize + this.tileSize / 2;
        const y = tileY * this.tileSize + this.tileSize / 2;

        const radius = this.tileSize;

        for (let i = 0; i < amount; i++) {
            const angle = Math.random() * 2 * Math.PI;
            const distance = Math.random() * radius;
            const randomVelocityX = Math.cos(angle) * distance;
            const randomVelocityY = Math.sin(angle) * distance;

            const length = Math.sqrt(randomVelocityX ** 2 + randomVelocityY ** 2);
            const normalizedVelocityX = randomVelocityX / length;
            const normalizedVelocityY = randomVelocityY / length;

            const targetAngle = Math.random() * 2 * Math.PI;
            const targetX = Math.cos(targetAngle) * radius;
            const targetY = Math.sin(targetAngle) * radius;

            const velocityX = (targetX - randomVelocityX) / 20;
            const velocityY = (targetY - randomVelocityY) / 20;

            this.entities.particles.push(
                new Particle(x, y,
                    normalizedVelocityX + velocityX,
                    normalizedVelocityY + velocityY,
                    this.gameTicks, color, aliveForTicks
                ));
        }
    }

    checkCollisions() {
        this.quadtree.clear();

        for (let monster of this.entities.monsters) {
            this.quadtree.insert(new Rectangle(monster.x * this.tileSize, monster.y * this.tileSize, this.tileSize, this.tileSize, monster));
        }

        this.entities.arrows.forEach(arrow => {
            let possibleCollisions = this.quadtree.query(
                new Rectangle(
                    arrow.x - this.tileSize / 2, 
                    arrow.y - this.tileSize / 2, 
                    this.tileSize , 
                    this.tileSize
                )
            );

            for (let possibleCollision of possibleCollisions) {
                const monster = possibleCollision.entity;
                const monsterX = monster.x * this.tileSize;
                const monsterY = monster.y * this.tileSize;
                const arrowX = arrow.x - this.tileSize / 2;
                const arrowY = arrow.y - this.tileSize / 2;
                const distance = Math.sqrt((arrowX - monsterX) ** 2 + (arrowY - monsterY) ** 2);
    
                if (arrow instanceof Bomb) {
                    if (distance < this.tileSize) {
                        const blastRadius = arrow.blastRadius

                        const possibleCollisions = this.quadtree.query(
                            new Rectangle(monster.x * this.tileSize - this.tileSize * blastRadius,
                                monster.y * this.tileSize - this.tileSize * blastRadius,
                                this.tileSize * blastRadius,
                                this.tileSize * blastRadius
                            ));

                        for (let possibleCollision of possibleCollisions) {
                            const m = possibleCollision.entity;
                            const distance = Math.sqrt((arrow.x - m.x * this.tileSize) ** 2 + (arrow.y - m.y * this.tileSize) ** 2);
                            if (distance < this.tileSize * 5) {
                                let damage = Math.floor(arrow.damage * (1 - distance / (this.tileSize * 10)));
                                this.damageMonster(m, damage);
                            }
                        };

                        this.entities.arrows = this.entities.arrows.filter(a => a !== arrow);
                        this.createParticlesAtTile(10, monster.x, monster.y, "black");
                        this.createParticlesAtTile(10, monster.x, monster.y, "grey");
                    }
                } else if (distance < this.tileSize) {
                    this.entities.arrows = this.entities.arrows.filter(a => a !== arrow);
                    this.damageMonster(monster, arrow.damage);
                }
            };
        });
    }

    damageMonster(monster, damage) {
        monster.hp -= damage;
        this.createParticlesAtTile(damage, monster.x, monster.y, "red");

        if (monster.hp <= 0) {
            this.createParticlesAtTile(monster.startHp, monster.x, monster.y, "red");
            this.gold += monster.startHp;
            this.entities.monsters = this.entities.monsters.filter(m => m !== monster);
        }
    }

    handleClick(event) {
        const x = Math.floor((event.clientX + this.cameraX) / this.tileSize);
        const y = Math.floor((event.clientY + this.cameraY) / this.tileSize);

        if (this.isPositionOccupied(x, y)) {
            const wallIndex = this.entities.walls.findIndex(wall => wall.x === x && wall.y === y);
            const towerIndex = this.entities.towers.findIndex(tower => tower.x === x && tower.y === y);

            if (event.shiftKey) {
                if (wallIndex !== -1) {
                    this.entities.walls.splice(wallIndex, 1);
                } else if (towerIndex !== -1) {
                    this.entities.towers.splice(towerIndex, 1);
                }
            } else if (event.ctrlKey) {
                if (wallIndex !== -1) {
                    this.entities.walls.splice(wallIndex, 1);
                    this.placeBuilding(new BombTower(x, y), x, y);
                } else if (towerIndex !== -1) {
                    this.entities.towers.splice(towerIndex, 1);
                    this.placeBuilding(new Wall(x, y, 'W'), x, y);
                }
            }
        } else if (event.ctrlKey) {
            this.placeBuilding(new Tower(x, y), x, y);
        } else {
            this.placeBuilding(new Wall(x, y, 'W'), x, y);
        }
    }

    placeBuilding(building, x, y) {
        if (this.isPositionOccupied(x, y)) return;

        if (building instanceof Tower || building instanceof BombTower) {
            this.entities.towers.push(building);
        } else if (building instanceof Wall) {
            this.entities.walls.push(building);
        }

        this.invalidateAllPaths();
    }

    handleKeydown(event) {
        const moveAmount = 10;
        switch (event.key) {
            case 'ArrowUp':
                this.cameraY -= moveAmount;
                break;
            case 'ArrowDown':
                this.cameraY += moveAmount;
                break;
            case 'ArrowLeft':
                this.cameraX -= moveAmount;
                break;
            case 'ArrowRight':
                this.cameraX += moveAmount;
                break;
        }
    }
}

new Game();