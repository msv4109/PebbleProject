Pebble.addEventListener("appmessage", function(e) {
sendToServer(e.payload.hello_msg); }
);


function sendToServer(str) {
var req = new XMLHttpRequest();
var message =  str;
var ipAddress = "165.123.219.160"; // Hard coded IP address
var port = "3002"; // Same port specified as argument to server 
var url = "http://" + ipAddress + ":" + port + "/" + message;
var method = "GET";
var async = true;
req.onload = function(e) {
// see what came back
var msg = "no response";
var response = JSON.parse(req.responseText); 
if (response) {
if (response.name) {
msg = response.name;
}
else msg = "noname"; 
}
if (req.readyState == 4 && req.status == 200) {
    console.log(msg);
    Pebble.sendAppMessage({"0": msg});
  } else {
    Pebble.sendAppMessage({"0": "Cannot connect to server"});
  }
// sends message back to pebble 
//Pebble.sendAppMessage({ "0": msg });
};
req.open(method, url, async); 
req.send(null);
}
/*Pebble.addEventListener("appmessage", function(e) {
if (e.payload) {
if (e.payload.hello_msg) {
Pebble.sendAppMessage({ "0": "Recvd: " + e.payload.hello_msg }); }
else Pebble.sendAppMessage({ "0": "nokey" }); }
else Pebble.sendAppMessage({ "0": "nopayload" }); }
);*/
