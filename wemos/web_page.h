#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char index_html[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Wemos Box</title>
      <style>
      html, body {
          font-family: 'Courier New', monospace;
          background-color: #0D0208;
          height: 100%;
          margin: 0;
          overflow: hidden;
      }

      body {
          display: flex;
          flex-direction: column;
      }

      h1 {
          color: #03A062;
      }

      h2 {
          color: #03A062;
          margin-top: 20px;
      }

      button, input {
          padding: 10px 20px;
          margin-top: 20px;
          margin-bottom: 20px;
          font-size: 14px;
          cursor: pointer;
          border: none;
          border-radius: 4px;
          background-color: #03A062;
          color: white;
          transition: background-color 0.3s;
      }

      form {
          border: 1px solid #03A062;
          border-radius: 8px;
      }

      .controller-button, .log-button {
        margin-top: 0px;
        margin-bottom: 10px;
        padding: 10px 20px;
        font-size: 16px;
        cursor: pointer;
        border: 1px solid #03A062;
        background-color: transparent;
        width: 50%;
      }

      .controller, .log-messages {
          flex: 1;
          overflow-y: auto;
          border: 1px solid #03A062;
          border-radius: 8px;
          padding: 10px;
          background-color: #0D0208;
          font-size: 12px;
          color: #03A062;
      }

      .controller *, .log-messages * {
          margin: 0px;
      }

      .hidden {
          display: none;
      }

      .button-active {
        background-color: #03A062;
      }

      .buttons-div {
        display: flex;
      }

      .status-indicator {
          padding: 10px 20px;
          cursor: pointer;
          border-radius: 4px;
          background-color: #4c4c4c;
          color: white;
          transition: background-color 0.3s;
      }

      .status-on {
          background-color: #03A062;
      }

      .status-off {
          background-color: #bd0000;
      }

      .controls, .bottom {
          flex: 1;
          min-height: 0;
          border-style: solid;
          border-color: #02683f;
          background-color: #0D0208;
          padding: 20px;
          border-radius: 8px;
          display: flex;
          flex-direction: column;
      }

      .controls {
          overflow-y: scroll;
      }

      #buttons-epson {
        padding-top: 10px;
        display: grid;
        grid-template-columns: repeat(2, 1fr); /* 2 items per row */
        gap: 10px;
      }

      .bigButtons {
        height: 100px;
      }

      </style>
  </head>
  <body>

      <div class="controls">
          <h1>Wemos Toolbox</h1>
          <button id="toggleIR" class="status-indicator">Toggle IR</button>
          <form id="sendIRForm">
              <button id="sendIRBtn" type="submit">Send IR</button>
              <input type="text" name="protocol" placeholder="protocol" id="protocolIR">
              <input type="text" name="address" placeholder="address" id="addressIR">
              <input type="text" name="command" placeholder="command" id="commandIR">
              <input type="text" name="repeats" placeholder="repeats" id="repeatsIR">
          </form>

          <form id="sendIRRawForm">
              <button id="sendIRRawBtn" type="submit">Send IR Raw</button>
              <input type="text" name="rawData" placeholder="rawData" id="rawIR">
              <input type="text" name="repeats" placeholder="repeats" id="repeatsIR">
          </form>

          <button id="toggleRadio" class="status-indicator">Toggle Radio</button>
          <form id="sendRadioForm">
              <button id="sendRadioBtn" type="submit">Send Radio</button>
              <input type="text" name="nPulseLength" placeholder="Pulse Length" id="nPulseLengthRadio">
              <input type="text" name="decimalCode" placeholder="Decimal Code" id="decimalCodeRadio">
              <input type="text" name="bitLength" placeholder="Bit Length" id="bitLengthRadio">
              <input type="text" name="protocol" placeholder="Protocol" id="protocolRadio">
          </form>

      </div>

      <div class="bottom">
          <div class="buttons-div">
              <button class="log-button button-active" id="log-button">Logs</button>
              <button class="controller-button" id="controller-button">Controller</button>
          </div>
          <div class="controller hidden" id="controller">
              <select id="selectController" name="selectController" class="controller" style="width: 100%">
                <option value="epson" selected>Epson</option>
              </select>
              <div id="buttons-epson" class="hidden">
                <button id="power" class="bigButtons">Power</button>
                <button id="freeze" class="bigButtons">Freeze</button>
                <button id="cursor" class="bigButtons">Cursor</button>
                <button id="getID" class="bigButtons">ID</button>
              </div>
          </div>
          <div class="log-messages" id="log-messages"></div>
      </div>

      <script>
          const toggleIRBtn = document.getElementById('toggleIR');
          const toggleRadioBtn = document.getElementById('toggleRadio');

          const repeatSignalBtn = document.getElementById('repeat-signal');

          const controllerBtn = document.getElementById('controller-button');
          const controller = document.getElementById('controller');

          const sendIRForm = document.getElementById('sendIRForm');
          const sendIRRawForm = document.getElementById('sendIRRawForm');
          const sendRadioForm = document.getElementById('sendRadioForm');

          const logMessagesBtn = document.getElementById('log-button');
          const logMessages = document.getElementById('log-messages');

          const buttonsEpson = document.getElementById('buttons-epson');
          const selectController = document.getElementById('selectController');

          const powerBtn = document.getElementById('power');

          powerBtn.addEventListener('click', function() {
            const selectedValue = selectController.value;
            if (selectedValue === "epson") {
              const formData = new FormData();
              formData.append("protocol", "necext");
              formData.append("address", "8355")
              formData.append("command", "906F");
              fetch('/sendIR', {
                method: 'POST',
                body: formData
              });
              updateLog();
            } else {
              console.log("Not selected.")
            }
          });

          const freezeBtn = document.getElementById('freeze');

          freezeBtn.addEventListener('click', function() {
            const selectedValue = selectController.value;
            if (selectedValue === "epson") {
              const formData = new FormData();
              formData.append("protocol", "necext");
              formData.append("address", "8355")
              formData.append("command", "926D");
              fetch('/sendIR', {
                method: 'POST',
                body: formData
              });
              updateLog();
            } else {
              console.log("Not selected.")
            }
          });

          const cursorBtn = document.getElementById('cursor');

          cursorBtn.addEventListener('click', function() {
            const selectedValue = selectController.value;
            if (selectedValue === "epson") {
              const formData = new FormData();
              formData.append("protocol", "necext");
              formData.append("address", "8355")
              formData.append("command", "9B64");
              fetch('/sendIR', {
                method: 'POST',
                body: formData
              });
              updateLog();
            } else {
              console.log("Not selected.")
            }
          });

          const getIDBtn = document.getElementById('getID');

          getIDBtn.addEventListener('click', function() {
            const selectedValue = selectController.value;
            if (selectedValue === "epson") {
              const formData = new FormData();
              formData.append("protocol", "necext");
              formData.append("address", "8355")
              formData.append("command", "31CE");
              fetch('/sendIR', {
                method: 'POST',
                body: formData
              });
              updateLog();
            } else {
              console.log("Not selected.")
            }
          });

          selectController.addEventListener("click", updateControllerMenu());
          selectController.addEventListener("change", updateControllerMenu());

          function updateControllerMenu() {
            const selectedValue = selectController.value;
            console.log(selectedValue);
            if (selectedValue === "epson") {
              // Open your menu here
              console.log("Open Epson menu");
              buttonsEpson.classList.remove("hidden");

            } else {
              buttonsEpson.classList.add("hidden");
            }
          }

          function getControllers() {
              fetch('/getControllers')
                  .then(response => response.json())
                  .then(data => {
                      console.log(data);

                      for (const fileName in data) {
                          console.log(fileName);
                      }

                  })
                  .catch(function(error) {
                      console.error('Could not connect to device.', 'error');
                  });
          }

          function updateLog() {
            fetch('/log')
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);
                      logMessages.innerHTML = '';

                      data.split('\n').forEach(line => {
                          if (line.trim() !== '') {
                              logMessages.innerHTML += `<p>${line}</p>`;
                          }
                      });
                      logMessages.scrollTop = logMessages.scrollHeight;
                  })
                  .catch(function(error) {
                      console.error('Could not connect to device.', 'error');
                  });
          }

          function updateStatus(data) {
              if (data.IRReceiveEnabled) {
                  toggleIRBtn.classList.remove('status-off');
                  toggleIRBtn.classList.add('status-on');
              } else {
                  toggleIRBtn.classList.remove('status-on');
                  toggleIRBtn.classList.add('status-off');
              }

              if (data.radioReceiveEnabled) {
                  toggleRadioBtn.classList.remove('status-off');
                  toggleRadioBtn.classList.add('status-on');
              } else {
                  toggleRadioBtn.classList.remove('status-on');
                  toggleRadioBtn.classList.add('status-off');
              }
          }

          logMessagesBtn.addEventListener('click', function() {
            controller.classList.add("hidden");
            logMessages.classList.remove("hidden");
            controllerBtn.classList.remove("button-active")
            logMessagesBtn.classList.add("button-active");

            updateLog();
          });

          controllerBtn.addEventListener('click', function() {
            logMessages.classList.add("hidden");
            controller.classList.remove("hidden");
            logMessagesBtn.classList.remove("button-active");
            controllerBtn.classList.add("button-active");
          });

          toggleIRBtn.addEventListener('click', function() {
              fetch('/toggleIR')
                  .then(response => response.text())
                  .then(text => {
                      updateLog(text);

                      return fetch('/status');
                  })
                  .then(response => response.json())
                  .then(data => {
                      updateStatus(data);
                  })
                  .catch(error => {
                      console.error('Error: ' + error, 'error');
                  });
          });

          toggleRadioBtn.addEventListener('click', function() {
              fetch('/toggleRadio')
                  .then(response => response.text())
                  .then(text => {
                      updateLog(text);

                      return fetch('/status');
                  })
                  .then(response => response.json())
                  .then(data => {
                      updateStatus(data);
                  })
                  .catch(error => {
                      console.error('Error: ' + error, 'error');
                  });
          });

          sendIRForm.addEventListener('submit', function(e) {
              e.preventDefault();

              fetch('/sendIR', {
                  method: 'POST',
                  body: new FormData(sendIRForm)
              })

              updateLog();
          });

          sendIRRawForm.addEventListener('submit', function(e) {
              e.preventDefault();

              fetch('/sendIRRaw', {
                  method: 'POST',
                  body: new FormData(sendIRRawForm)
              })

              updateLog();
          });

          sendRadioForm.addEventListener('submit', function(e) {
              e.preventDefault();

              fetch('/sendRadio', {
                  method: 'POST',
                  body: new FormData(sendRadioForm)
              })

              updateLog();
          });

          setInterval(function() {
              fetch('/status')
                  .then(function(response) { return response.json(); })
                  .then(function(data) {
                      updateStatus(data);
                  })
                  .catch(function(error) {
                      console.error('Status error:', error);
                  });
          }, 3000);

          window.addEventListener('load', function() {
              fetch('/status')
                  .then(function(response) { return response.json(); })
                  .then(function(data) {
                      console.log('Connected to device!', 'success');
                      updateStatus(data);
                  })
                  .catch(function(error) {
                      console.error('Could not connect to device.', 'error');
                  });
          });

          setInterval(updateLog, 2000);
      </script>
  </body>
  </html>
)=====";

#endif
