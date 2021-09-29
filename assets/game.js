function playAudio(audio)
{
    var audio = new Audio('wave/' + audio + '.wav');
    audio.play();
}

function createImage(width)
{
    var image = document.getElementById('image');
    CallHandler('body', 'image', width + ' ' +
        Math.round(image.offsetWidth * width / image.offsetHeight));
}

function escape()
{
    CallHandler('body', 'escape', '');
}

function setImage(src)
{
    document.getElementById('image').setAttribute('src', 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==');
}

function clickImage()
{
    CallHandler('body', 'click', '');
}
