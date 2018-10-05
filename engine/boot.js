
import Engine from 'engine.js';
import Physics from 'phys/simple.js';
import Renderer from 'render/text.js';

// TODO This should be in context.
let entities = [];

core.addSystem(new Engine(entities));
core.addSystem(new Physics(entities));
core.addSystem(new Renderer(entities));

let A = {
  position: {
    x: 10, y: 10, z: 0
  },
  textCells: [
    [' ','*',' '],
    ['*','*','*'],
    [' ','*',' '],
  ]
}
entities.push(A);