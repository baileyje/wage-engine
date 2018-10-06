
class SimplePhysics {

  constructor(entities, config = {}) {
    this.entities = entities;
  }

  init() {    
    console.log('Initializing Simple Physics');
  }

  start() {
  }

  update(ctx) {
  }

  integrate(ctx) {
    this.entities.forEach(entity => {
      entity.velocity.x += entity.acceleration.x * ctx.timeStep;
      entity.velocity.y += entity.acceleration.y * ctx.timeStep;
      entity.position.x += entity.velocity.x * ctx.timeStep;
      entity.position.y += entity.velocity.y * ctx.timeStep;
    }); 
  }

  colisionDetection(ctx) {
    this.entities.forEach(a => {
      this.entities.forEach(b => {
        if (a === b) return;
        if (a.position.x < b.position.x + 3/*player2.width*/ && 
          a.position.x + 3 /*player1.width*/ > b.position.x &&
          a.position.y < b.position.y + 3 /*player2.height*/ && 
          a.position.y + 3 /*player1.height*/ > b.position.y) {
          console.log("Collision Detected");
          a.velocity = {x: 0, y: 0, z: 0};
          b.velocity = {x: 0, y: 0, z: 0};
        }
      });
    });
  }

  fixedUpdate(ctx) {
    this.integrate(ctx);
    this.colisionDetection(ctx);
  }

  stop() {
  }

  deinit() {
    console.log('Deinitializing Simple Physics');
  }

}

export default SimplePhysics;

