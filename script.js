var url = 'https://HOST-ADDRESS.000webhostapp.com/api/in/specific.php';
var timer = 0;

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