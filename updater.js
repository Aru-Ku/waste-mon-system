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
// alert(dry + wet);

function fun() {
    document.getElementById("dryy").style.width = "20%";
    document.getElementById("dryy").innerHTML = "20%";
}