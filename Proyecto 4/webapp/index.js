var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.onload = function(){initWebSocket();};

function initWebSocket()
{
    websocket = new WebSocket(gateway);
    websocket.onmessage = onMessage;
};

function onMessage(event)
{
    let data = JSON.parse(event.data);
    console.log(`Received a notification from ${event.origin}`);
    console.log("LED1: " + data.led1);
    console.log("LED2: " + data.led2);
    console.log("LED3: " + data.led3);
    console.log("LED4: " + data.led4);
    displayColor("car1", data.led1);
    displayColor("car2", data.led2);
    displayColor("car3", data.led3);
    displayColor("car4", data.led4);
};

function displayColor(id, value)
{
    var element = document.getElementById(id);

    if (value)
    {
        element.style.backgroundColor = "#66ff66";
    }
    else
    {
        element.style.backgroundColor = "#ff6666";
    }
}