var widths_ = [];
var heights_ = [];

var mouseX = 0;
var mouseY = 0;

var audios_= [];
const AudioContext = window.AudioContext || window.webkitAudioContext;
var audioCtx;

function setup()
{
    var ids = ['food', 'turn', 'move', 'win', 'die'];
    if (AudioContext)
    {
        audioCtx = new AudioContext();
    }
    var loads = ids.map(function(id)
    {
        if (!audioCtx)
        {
            return Promise.reject();
        }
        return new Promise(function(resolve, reject)
        {
            var request = new XMLHttpRequest();
            request.open('GET', cross_asset_domain_ + 'wave/' + id + '.wav',
                cross_asset_async_);
            request.responseType = 'arraybuffer';
            request.onload = function()
            {
                audioCtx.decodeAudioData(
                    request.response,
                    function(buffer)
                    {
                        audios_[id] = buffer;
                        resolve();
                    },
                    reject);
            };
            request.onerror = reject;
            request.send();
        });
    });
    Promise.allSettled(loads).then(function()
    {
        notifyResize('image');
    });
}

function playAudio(id)
{
    if (!audioCtx || !audios_[id])
    {
        return;
    }
    if (audioCtx.state === 'suspended')
    {
        audioCtx.resume();
    }
    var source = audioCtx.createBufferSource();
    source.buffer = audios_[id];
    source.connect(audioCtx.destination);
    source.start(0);
}

function escape()
{
    CallHandler('body', 'escape', '');
}

function touchStart(event)
{
    if (cross_pointer_type_ == 'touch')
    {
        mouseX = event.touches[0].screenX;
        mouseY = event.touches[0].screenY;
    }
}

function mouseDown(event)
{
    if (cross_pointer_type_ == 'mouse')
    {
        mouseX = event.screenX;
        mouseY = event.screenY;
    }
}

function touchEnd(event)
{
    if (cross_pointer_type_ == 'touch')
    {
        var mX = event.changedTouches[0].screenX - mouseX;
        var mY = event.changedTouches[0].screenY - mouseY;
        if (cross_pointer_upsidedown_)
        {
            mY = -mY;
        }
        CallHandler('game', 'drag', Math.round(mX) + ' ' + Math.round(mY));
    }
}

function mouseUp(event)
{
    if (cross_pointer_type_ == 'mouse')
    {
        var mX = event.screenX - mouseX;
        var mY = event.screenY - mouseY;
        if (cross_pointer_upsidedown_)
        {
            mY = -mY;
        }
        CallHandler('game', 'drag', Math.round(mX) + ' ' + Math.round(mY));
    }
}

function keyDown(event)
{
    if (event.repeat)
    {
        return;
    }
    var mX = 0;
    var mY = 0;
    switch (event.key)
    {
    case 'ArrowLeft':
    case 'a':
    case 'A':
        mX = -1;
        break;
    case 'ArrowRight':
    case 'd':
    case 'D':
        mX = 1;
        break;
    case 'ArrowUp':
    case 'w':
    case 'W':
        mY = -1;
        break;
    case 'ArrowDown':
    case 's':
    case 'S':
        mY = 1;
        break;
    case ' ':
    case 'Enter':
        break;
    default:
        return;
    }
    event.preventDefault();
    if (cross_pointer_upsidedown_)
    {
        mY = -mY;
    }
    CallHandler('game', 'drag', mX + ' ' + mY);
}

function resetImage(receiver, index, id)
{
    document.getElementById(id).setAttribute('src',
        'cross://' + receiver + '/' + id + '/' + index);
}

function notifyResize(command)
{
    var image = document.getElementById('image');
    heights_['image'] = Math.round(widths_['image'] * image.offsetHeight / image.offsetWidth);
    CallHandler('body', command,  widths_['image'] + ' ' + heights_['image']);
}

function showBack(state)
{
    if (state)
    {
        document.getElementById('escape').removeAttribute('data-transparent');
    }
    else
    {
        document.getElementById('escape').setAttribute('data-transparent', '');
    }
}
