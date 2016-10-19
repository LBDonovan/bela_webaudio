/***** index.js *****/
var AudioContext = require('./AudioContext');

var ctx = new AudioContext();

var splitter = ctx.createChannelSplitter(2);
var merger = ctx.createChannelMerger(2);

var gainl = ctx.createGain(10);
var gainr = ctx.createGain(1);

ctx.source.connect(splitter);

splitter.connect(gainl, 0);
splitter.connect(gainr, 1);

gainl.connect(merger, 0, 0);
gainr.connect(merger, 0, 1);

merger.connect(ctx.destination);