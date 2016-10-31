/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var gain = ctx.createGain(1);

ctx.source.connect(gain);
gain.connect(ctx.destination);

var oscillator = ctx.createOscillator();
oscillator.type = 'sine';
oscillator.frequency.value = 2; // value in hertz
oscillator.connect(gain.gain);
oscillator.start();

var oscillator2 = ctx.createOscillator();
oscillator2.type = 'sine';
oscillator2.frequency.value = 3; // value in hertz
oscillator2.connect(gain.gain);
oscillator2.start();

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
		// oscillator.frequency.value = 220;
	}, 6000);
}