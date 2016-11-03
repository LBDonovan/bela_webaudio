/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var osc = ctx.createOscillator();
osc.frequency.value = 440;
osc.type = 'sine';
osc.start();

osc.connect(ctx.destination);

ctx.onstatechange = function() {
  console.log('STATE', ctx.state);
  setTimeout( () => {
  	osc.type = 'square';
  }, 5000);
  setTimeout( () => {
  	osc.type = 'sawtooth';
  }, 10000);
  setTimeout( () => {
  	osc.type = 'triangle';
  }, 15000);
  setTimeout( () => {
  	osc.stop();
  }, 20000);
}