
import Engine from 'engine.js';


core.engine = new Engine();
const prom = new Promise(function(resolve, reject) {
  core.engine.init();
  resolve();
});
prom.then(function() {
 console.log("Resolved!");
});

