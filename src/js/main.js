console.log("Hello world!");

const FPS = 45;
const FRAME_DURATION = 1000 / FPS;
const INPUT = {
    up: false,
    down: false,
    left: false,
    right: false,
    space: false,
    esc: false,
    enter: false,
};
const CANVAS = document.getElementById("gameCanvas");
const PAUSE_MENU = document.getElementById("pauseMenu");
const GAMEOVER_MENU = document.getElementById("gameOverMenu");
const MAIN_MENU = document.getElementById("mainMenu");

let lives = 3;
const LIVES_TEXT = document.getElementById("livesText");
let score = 0;
let best_score = 0;
const SCORE_TEXT = document.getElementById("scoreText");

const GAMEOVER_SCORE_TEXT = document.getElementById("gameOverScore");
const GAMEOVER_BEST_SCORE_TEXT = document.getElementById("gameOverTopScore");

PAUSE_MENU.style.visibility = "hidden";
GAMEOVER_MENU.style.visibility = "hidden";


/**
 * Adding event listeners
 */
setTimeout(() => {
    window.addEventListener("keydown", (e) => {
        let key = e.key;
        let code = key.charCodeAt(0);
        if(key.length === 1) {
            _key_pressed(code);
            return;
        }
        else if(key === "Enter") {
            code = '\n'.charCodeAt(0);
        }
        else if(key === "Escape") {
            code = 27;
        }
        else if(key === "Shift") {
            code = 0;
        }
        _key_pressed(code);
    });
    
    window.addEventListener("keyup", (e) => {
        let key = e.key;
        let code = key.charCodeAt(0);
        if(key.length === 1) {
            _key_released(code);
            return;
        }
        else if(key === "Enter") {
            code = '\n'.charCodeAt(0);
        }
        else if(key === "Escape") {
            code = 27;
        }
        else if(key === "Shift") {
            code = 0;
        }
        _key_released(code);
    });
},500);

/**
 * Animation loop
 */
let zero = performance.now();
const animate = () => {
    const value = (performance.now() - zero) / FRAME_DURATION;
    if(value >= 1) {
        zero = performance.now();
        _get_frame();
    }
    requestAnimationFrame(animate);
};

setTimeout(() => {
    _init();
    requestAnimationFrame(animate);
}, 50);

/**
 * Game Functionality
 */
window.sendSignal = (signal, data) => {
    if(data !== undefined) {
        switch(signal) {
            case "scoreUpdated":
                SCORE_TEXT.innerText = data.toString().padStart(4, "0");
                score = data;
                break;
            case "livesUpdated":
                LIVES_TEXT.innerText = data.toString();
                lives = data;
                break;
            default:
                console.log("Unknown data");
                break;
        }
        return;
    }
    switch(signal) {
        case "pause":
            PAUSE_MENU.style.visibility = "visible";
            break;
        case "resume":
            PAUSE_MENU.style.visibility = "hidden";
            break;
        case "start":
            MAIN_MENU.style.visibility = "hidden";
            break;
        case "restart":
            MAIN_MENU.style.visibility = "hidden";
            PAUSE_MENU.style.visibility = "hidden";
            GAMEOVER_MENU.style.visibility = "hidden";
            break;
        case "quit":
            GAMEOVER_MENU.style.visibility = "hidden";
            PAUSE_MENU.style.visibility = "hidden";
            MAIN_MENU.style.visibility = "visible";
            break;
        case "score":
            score += 1;
            SCORE_TEXT.innerText = score.toString().padStart(4, "0");
            break;
        case "damage":
            break;
        case "gameOver":
            GAMEOVER_MENU.style.visibility = "visible";
            GAMEOVER_SCORE_TEXT.innerText = score;
            if(score > best_score) {
                best_score = score;
                GAMEOVER_BEST_SCORE_TEXT.innerText = best_score;
            }
            break;
        default:
            console.log("Unknown signal");
            break;
    }
}