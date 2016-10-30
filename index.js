/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var oscillator = ctx.createOscillator();

oscillator.type = 'sine';
oscillator.frequency.value = 440; // value in hertz
oscillator.connect(ctx.destination);
oscillator.start();

ctx.onstatechange = function() {
  console.log('STATE', ctx.state);
  //setInterval( () => console.log(ctx.currentTime), 1000);
  setTimeout( () => {
		// console.log('setting gain');
		oscillator.detune.value = 50;
	}, 2000);
	setTimeout( () => {
		// console.log('setting gain');
		oscillator.detune.value = -50;
	}, 4000);
	setTimeout( () => {
		// console.log('setting gain');
		oscillator.detune.value = 0;
		oscillator.frequency.value = 220;
	}, 6000);
}