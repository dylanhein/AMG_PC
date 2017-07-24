const raspi = require('raspi-io');
const five = require('johnny-five');
const path = require('path');
const express = require('express');
const app = express();

var server = require('http').Server(app);
var socketio = require('socket.io')(server, {
  origins: 'localhost:* http://localhost:* 127.0.0.0:*'
});

const PORT = 8001;
const feetPerMeter = 3.28084;
const board = new five.Board({
  io: new raspi()
});

board.on('ready', () => {
  var bmp180 = new five.Multi({
    controller: 'BMP180',
    address: 0x77
  });
  
  server.listen(PORT, (req, res) => {
    console.log('Listening on port: ', PORT);
  });
  
  app.use(express.static(path.join(__dirname +'/public')));
  app.use('/bower_components', express.static(path.join(__dirname + '/bower_components')));
  app.use('/img', express.static(path.join(__dirname + '/bower_components/jquery-flight-indicators/img')));
  
  socketio.on('connection', (socket) => {
    console.log('Connection created.');
	
    var previousTemp = 0;
    var previousPressure = 0;
    
    bmp180.on('change', (data) => {
      if (Math.abs(data.thermometer.fahrenheit - previousTemp) > 1 ||
          Math.abs(data.barometer.pressure - previousPressure) > 1) {      
        previousTemp = data.thermometer.fahrenheit;
        previousPressure = data.barometer.pressure;
        
        //Pressure is returned in kPa from sensor
        //Return pressure value in hPa
        //hPa = kPa * 10
        console.log('Temp: ', data.thermometer.fahrenheit);
        console.log('Pressure: ', data.barometer.pressure);
        
        var pressure = data.barometer.pressure * 10;
        var seaLevelPressure = 1013.25;
        var pressureDiff = seaLevelPressure / pressure;
        var altitudeFirstPart = (Math.pow(pressureDiff, (1 / 5.257)) - 1)
        var altitudeSecondPart = data.thermometer.celsius + 273.15;
        var altitude = (altitudeFirstPart * altitudeSecondPart) / 0.0065;
        altitude = altitude * feetPerMeter;
        
        var alt2 = 44330 * (1 - Math.pow((pressure / seaLevelPressure), (1 / 5.255)));
        alt2 = alt2 * feetPerMeter;
        
        console.log('Altitude: ', altitude);
        console.log('Altitude2: ', alt2);
        
        socket.emit('pressure', {
          pressure: pressure,
          altitude: altitude
        });
      }
    }); 
  });
  
});
