
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
    x: 10, y: 11, z: 0
  },
  velocity: {
    x: 3, y: 0, z: 0
  },
  acceleration: {
    x: 0, y: 0, z: 0
  },
  textCells: [
    [' ','*',' '],
    ['*','A','*'],
    [' ','*',' '],
  ], 
  // update: (ctx) => {
  //   // console.log('Update - ', ctx.deltaTime);
  //   A.position.x += 2 * ctx.timeStep;
  //   A.position.y += 1 * ctx.timeStep;
  //   // console.log('Update - ', A.position.x);
  // }
}
entities.push(A);

let B = {
  position: {
    x: 70, y: 11, z: 0
  },
  velocity: {
    x: -3, y: 0, z: 0
  },
  acceleration: {
    x: 0, y: 0, z: 0
  },
  textCells: [
    [' ','*',' '],
    ['*','B','*'],
    [' ','*',' '],
  ], 
  // update: (ctx) => {
  //   // console.log('Update - ', ctx.deltaTime);
  //   A.position.x += 2 * ctx.timeStep;
  //   A.position.y += 1 * ctx.timeStep;
  //   // console.log('Update - ', A.position.x);
  // }
}
entities.push(B);