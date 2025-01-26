const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chimenea</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 0;
            padding: 0;
            background-color: #f4f4f9;
        }
        h1 {
            margin: 20px;
            font-size: 2rem;
            color: #333;
        }
        .button-container {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            max-width: 300px;
            margin: 0 auto;
        }
        button {
            padding: 15px;
            font-size: 1.5rem;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            background-color: #007bff;
            color: #fff;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #0056b3;
        }
        .wide {
            grid-column: span 3;
        }
    </style>
</head>
<body>
    <h1>Chimenea2000</h1>
    <div class="button-container">
        <button onclick="sendCommand('previous')"><<</button>
        <button onclick="sendCommand('play')">▶</button>
        <button onclick="sendCommand('next')">>></button>
        <button class="wide" onclick="sendCommand('volumeup')">Volume ↑</button>
        <button class="wide" onclick="sendCommand('volumedown')">Volume ↓</button>
        <button class="wide" onclick="sendCommand('stop')">■ Stop</button>
    </div>
    <script>
        function sendCommand(command) {
            fetch(`/${command}`)
                .then(response => {
                    if (response.ok) {
                        console.log(`${command} command sent successfully`);
                    } else {
                        console.error(`Failed to send ${command} command`);
                    }
                })
                .catch(error => {
                    console.error(`Error: ${error}`);
                });
        }
    </script>
</body>
</html>
)=====";

