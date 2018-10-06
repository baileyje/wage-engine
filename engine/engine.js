class Engine {

  constructor(entities, config = {}) {
    this.entities = entities;
  }

  init() {    
    console.log('Initializing Engine');
  }

  start() {
    if ( this.running ) return;
    console.log('Starting Engine');
  }

  update(ctx) {
    // console.log('Update');
  }

  fixedUpdate(ctx) {
    // console.log('Fixed Update');
    this.entities.forEach(entity => {
      if (entity.hasOwnProperty('update')) {
        entity.update.call(entity, ctx);
      }
    });
  }

  stop() {
    console.log('Stopping Engine');
  }

  deinit() {
    console.log('Deinitializing Engine');
  }

}

export default Engine;