var url = 'https://iotgarbagemonitoringsystem.000webhostapp.com/api/in/specific.php';

function Get(url) {
    var Httpreq = new XMLHttpRequest();
    Httpreq.open("GET", url, false);
    Httpreq.send(null);
    return Httpreq.responseText;
}
var line = Get(url);
obj = JSON.parse(line);
var dry = obj.receiver[0].drylevel;
var wet = obj.receiver[0].wetlevel;

function fun() {
    document.getElementById("dry-bar").setAttribute("value", dry);
    document.getElementById("wet-bar").setAttribute("value", wet);
    document.getElementById("dry-bar-level").innerHTML = dry + " %";
    document.getElementById("wet-bar-level").innerHTML = wet + " %";
}

$(document).ready(
    function () {
        setInterval(function () {
            var someval = Math.floor(Math.random() * 100);
            $('#display');
        }, 5000);
    }
);