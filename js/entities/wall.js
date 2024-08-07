import { Building } from './building.js';

export class Wall extends Building {
    constructor(x, y) {
        super(x, y, 'W');
        this.hp = 5;
        this.cost = 5;

        this.connections = {
            north: false,
            east: false,
            south: false,
            west: false
        };
    }

    getRenderData() {
        // check the connections and return the correct image
        // check 4 connections first
        if (this.connections.north && this.connections.east && this.connections.south && this.connections.west) {
            return 'wall-all';
        }

        // three connections
        if (this.connections.north && this.connections.east && this.connections.west) {
            return 'wall-three-top';
        } else if (this.connections.north && this.connections.east && this.connections.south) {
            return 'wall-three-right';
        } else if (this.connections.north && this.connections.south && this.connections.west) {
            return 'wall-three-left';
        } else if (this.connections.east && this.connections.south && this.connections.west) {
            return 'wall-three-bottom';
        }

        // vertical and horizontal walls go first       
        if (this.connections.north && this.connections.south) {
            return 'vertical-wall';
        } else if (this.connections.east && this.connections.west) {
            return 'horizontal-wall';
        }

        // corners
        if (this.connections.north && this.connections.east) {
            return 'corner-wall-top-right';
        } else if (this.connections.north && this.connections.west) {
            return 'corner-wall-top-left';
        } else if (this.connections.south && this.connections.east) {
            return 'corner-wall-bottom-right';
        } else if (this.connections.south && this.connections.west) {
            return 'corner-wall-bottom-left';
        }

        // only one connection
        if (this.connections.north) {
            return 'wall-north';
        } else if (this.connections.east) {
            return 'wall-east';
        } else if (this.connections.south) {
            return 'wall-south';
        } else if (this.connections.west) {
            return 'wall-west';
        }

        // no connections
        return 'wall';

    }
}