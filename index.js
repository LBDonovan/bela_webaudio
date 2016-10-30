/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var gain = ctx.createGain(5);
ctx.source.connect(gain);
gain.connect(ctx.destination);

ctx.onstatechange = function() {
  console.log('STATE', ctx.state);
  //setInterval( () => console.log(ctx.currentTime), 1000);
  setTimeout( () => {
		console.log('setting gain');
		gain.gain.value = 0.1;
	}, 2000);
}