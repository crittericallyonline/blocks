import * as THREE from "three";
var Game;
(function (Game) {
    let Keyboard;
    (function (Keyboard) {
        const array = [];
        function down(ev) {
            const key = array.indexOf(ev.key);
            if (key > -1)
                return;
            array.push(ev.key);
        }
        function up(ev) {
            const key = array.indexOf(ev.key);
            array.splice(key, 1);
        }
        window.addEventListener("keydown", down);
        window.addEventListener("keyup", up);
        function pressed(key) {
            return array.indexOf(key) > -1;
        }
        Keyboard.pressed = pressed;
    })(Keyboard || (Keyboard = {}));
    let Chunks;
    (function (Chunks) {
        Chunks.size = 2;
        const chunks = [];
        function generate() {
            const c = new Uint8Array(Math.pow(Chunks.size, 3));
            chunks.push(c);
            c.fill(13);
            return c;
        }
        Chunks.generate = generate;
    })(Chunks || (Chunks = {}));
    let Assets;
    (function (Assets) {
        Assets.scene = new THREE.Scene();
        Assets.camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight);
        Assets.renderer = new THREE.WebGLRenderer();
        Assets.TexLoader = new THREE.TextureLoader();
        Assets.renderer.setSize(window.innerWidth, window.innerHeight);
        Assets.camera.rotation.order = "YXZ";
        document.body.appendChild(Assets.renderer.domElement);
    })(Assets || (Assets = {}));
    let Player;
    (function (Player) {
        const floory = 0;
        class InnerPlayer {
            position;
            oldPos;
            rotation;
            velocity;
            size;
            gravity = 0.0025;
            speed = 0.0075;
            jumpPower = 0.15;
            onFloor = false;
            constructor() {
                this.position = new THREE.Vector3(0);
                this.oldPos = new THREE.Vector3(0);
                this.rotation = new THREE.Vector3(0);
                this.size = new THREE.Vector3(0.75, 0.80, 0.75);
                this.velocity = new THREE.Vector3(0);
                this.onFloor = false;
            }
            update() {
                if (Keyboard.pressed('w')) {
                    this.velocity.z -= Math.cos(Assets.camera.rotation.y) * this.speed;
                    this.velocity.x -= Math.sin(Assets.camera.rotation.y) * this.speed;
                }
                if (Keyboard.pressed('s')) {
                    this.velocity.z += Math.cos(Assets.camera.rotation.y) * this.speed;
                    this.velocity.x += Math.sin(Assets.camera.rotation.y) * this.speed;
                }
                if (Keyboard.pressed('a')) {
                    this.velocity.z += Math.sin(Assets.camera.rotation.y) * this.speed;
                    this.velocity.x -= Math.cos(Assets.camera.rotation.y) * this.speed;
                }
                if (Keyboard.pressed('d')) {
                    this.velocity.z -= Math.sin(Assets.camera.rotation.y) * this.speed;
                    this.velocity.x += Math.cos(Assets.camera.rotation.y) * this.speed;
                }
                Assets.camera.position.set(this.position.x, this.position.y + this.size.y / 2 + 3, this.position.z);
                this.velocity.multiply(new THREE.Vector3(0.85, 1, 0.85));
                this.velocity.y -= this.gravity;
                this.oldPos = this.position.clone();
                this.position.add(this.velocity);
                if (this.position.y - this.size.y / 2 < floory) {
                    this.position.y = floory + this.size.y / 2;
                    this.velocity.y = 0;
                    this.onFloor = true;
                }
                else {
                    this.onFloor = false;
                }
                if (this.onFloor && Keyboard.pressed(" ")) {
                    this.velocity.y = this.jumpPower;
                    this.onFloor = false;
                }
            }
        }
        function create() {
            return new InnerPlayer();
        }
        Player.create = create;
    })(Player || (Player = {}));
    const player = Player.create();
    const gameObjects = [];
    const map = Assets.TexLoader.load("../res/tex/blocks.png", (tex) => {
        tex.minFilter = THREE.NearestFilter;
        tex.magFilter = THREE.NearestFilter;
        tex.wrapS = THREE.RepeatWrapping;
        tex.wrapT = THREE.RepeatWrapping;
        tex.repeat.x = 1 / 8;
        tex.repeat.y = 1 / 8;
        tex.offset.x = 5 / 8;
        tex.offset.y = 1 / 8;
        tex.flipY = false;
    });
    let chunkMesh = new THREE.Mesh(new THREE.BufferGeometry(), new THREE.MeshBasicMaterial({ map, wireframe: true, vertexColors: false, transparent: true }));
    const chunk = Chunks.generate();
    function getBlock(chunk, x, y, z) {
        const size = Chunks.size;
        return chunk[x + (y * size) + (z * (size * size))];
    }
    const num_vec3 = 3;
    const num_uv = 2;
    let initialBlock = getBlock(chunk, 0, 0, 0);
    let right = 0;
    let up = 0;
    let forward = 0;
    if (getBlock(chunk, 1, 0, 0) == initialBlock) {
        right++;
    }
    if (getBlock(chunk, 0, 1, 0) == initialBlock) {
        up++;
    }
    if (getBlock(chunk, 0, 0, 1) == initialBlock) {
        forward++;
    }
    const positionBuffer = new Float32Array(3 * 8);
    const uvBuffer = new Float32Array(2 * (3 * 4));
    uvBuffer[0] = 0;
    uvBuffer[1] = 0;
    uvBuffer[2] = 1;
    uvBuffer[3] = 0;
    uvBuffer[4] = 0;
    uvBuffer[5] = 1;
    uvBuffer[6] = 1;
    uvBuffer[7] = 1;
    uvBuffer[8] = 0;
    uvBuffer[9] = 0;
    uvBuffer[10] = 1;
    uvBuffer[11] = 0;
    uvBuffer[12] = 0;
    uvBuffer[13] = 1;
    uvBuffer[14] = 1;
    uvBuffer[15] = 1;
    positionBuffer[0] = -1;
    positionBuffer[1] = 1 + up;
    positionBuffer[2] = 1;
    positionBuffer[3] = 1 + right;
    positionBuffer[4] = 1 + up;
    positionBuffer[5] = 1;
    positionBuffer[6] = -1;
    positionBuffer[7] = -1;
    positionBuffer[8] = 1;
    positionBuffer[9] = 1 + right;
    positionBuffer[10] = -1;
    positionBuffer[11] = 1;
    positionBuffer[12] = 1 + right;
    positionBuffer[13] = 1 + up;
    positionBuffer[14] = -1 - forward;
    positionBuffer[15] = -1;
    positionBuffer[16] = 1 + up;
    positionBuffer[17] = -1 - forward;
    positionBuffer[18] = 1 + right;
    positionBuffer[19] = -1;
    positionBuffer[20] = -1 - forward;
    positionBuffer[21] = -1;
    positionBuffer[22] = -1;
    positionBuffer[23] = -1 - forward;
    chunkMesh.geometry.setAttribute("position", new THREE.BufferAttribute(positionBuffer, num_vec3));
    chunkMesh.geometry.setAttribute("uv", new THREE.BufferAttribute(uvBuffer, num_uv));
    chunkMesh.geometry.setIndex([
        //front
        3, 1, 2,
        2, 1, 0,
        //back
        5, 4, 7,
        7, 4, 6,
        //left
        0, 5, 7,
        0, 7, 2,
        //right
        1, 3, 4,
        4, 3, 6
    ]);
    chunkMesh.position.z = -5;
    Assets.scene.add(chunkMesh);
    gameObjects.push(chunkMesh);
    function gameLoop() {
        player.update();
        Assets.renderer.render(Assets.scene, Assets.camera);
    }
    Assets.renderer.setAnimationLoop(gameLoop);
    const sensitivity = 0.0025;
    function move(ev) {
        Assets.camera.rotation.x -= ev.movementY * sensitivity;
        Assets.camera.rotation.y -= ev.movementX * sensitivity;
    }
    document.addEventListener('mousemove', move);
})(Game || (Game = {}));
