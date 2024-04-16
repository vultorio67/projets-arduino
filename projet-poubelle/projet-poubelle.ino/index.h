/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<!-- saved from url=(0019)http://192.168.0.2/ -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>ESP8266 WebSocket</title>
<meta name="viewport" content="width=device-width, initial-scale=0.7">
<link rel="icon" href="https://diyables.io/images/page/diyables.svg">

  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    h1 {
      margin-top: 50px;
    }
    .container {
      margin-top: 20px;
    }
    .button {
      padding: 10px 20px;
      margin: 10px;
      font-size: 16px;
      cursor: pointer;
    }
    #text {
      margin-top: 20px;
    }
  </style>


<script>
var ws;
var wsm_max_len = 4096; /* bigger length causes uart0 buffer overflow with low speed smart device */

function update_text(text) {
  var chat_messages = document.getElementById("chat-messages");
  chat_messages.innerHTML += text + '<br>';
  chat_messages.scrollTop = chat_messages.scrollHeight;
}

function send_onclick() {
  if(ws != null) {
      ws.send("msg" + "\n");
  }
}

function connect_onclick() {
  if(ws == null) {
    ws = new WebSocket("ws://" + window.location.host + ":81");
    document.getElementById("ws_state").innerHTML = "CONNECTING";
    ws.onopen = ws_onopen;
    ws.onclose = ws_onclose;
    ws.onmessage = ws_onmessage;
  } else
    ws.close();
}

function ws_onopen() {
  document.getElementById("ws_state").innerHTML = "<span style='color:blue'>CONNECTED</span>";
  document.getElementById("bt_connect").innerHTML = "Disconnect";
  document.getElementById("chat-messages").innerHTML = "";
}

function ws_onclose() {
  document.getElementById("ws_state").innerHTML = "<span style='color:gray'>CLOSED</span>";
  document.getElementById("bt_connect").innerHTML = "Connect";
  ws.onopen = null;
  ws.onclose = null;
  ws.onmessage = null;
  ws = null;
}

function ws_onmessage(e_msg) {
  e_msg = e_msg || window.event; // MessageEvent
  console.log(e_msg.data);
  update_text('<span style="color:blue">' + e_msg.data + '</span>');
}
</script>
</head>
<body>
  <h1>Robot Poubelle</h1>
  <div class="container">
    <button class="button" onclick="send_onclick("on")">Position 1</button>
    <button class="button" onclick="send_onclick("off")">Position 2</button>
  </div>
  <div id="text">
    <p>Statut : En attente de commande...</p>
  </div>

      <div class="websocket">
      <button class="connect-button" id="bt_connect" onclick="connect_onclick()">Connect</button>
      <span class="label">WebSocket: <span id="ws_state"><span style="color:blue">CLOSED</span></span></span>
    </div>

  <script>
    function position1() {
      document.getElementById("text").innerHTML = "Position 1 activée";
    }

    function position2() {
      document.getElementById("text").innerHTML = "Position 2 activée";
    }
  </script>
</body>

</html>
)=====";
