function play()
{
    CallHandler("play", "click", "");
}

function reset()
{
    CallHandler("reset", "click", "");
}

function setLives(lives)
{
    for (var i = 0; i < lives; ++i)
        document.getElementById("lives").appendChild(document.createElement("div"));
}

function setLevel(level)
{
    document.getElementById("level").innerText = level;
}

function setScore(score)
{
    document.getElementById("score").innerText = score;
}

function setSound(sound)
{
    document.getElementById("sound").checked = sound;
}

function sound()
{
    CallHandler("sound", "click", document.getElementById("sound").checked.toString());
}
