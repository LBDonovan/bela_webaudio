/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var gain = ctx.createGain(1);

ctx.source.connect(gain);
gain.connect(ctx.destination);

gain.gain.setValueAtTime(10, 2);
gain.gain.setValueAtTime(1, 4);

ctx.onstatechange = function() {
  console.log('STATE', ctx.state);
}