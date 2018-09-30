
class Component {
  
  start(engine) {
    if (this.running) return;
    this.engine = engine;
    this.running = true;    
    onStop();
  }

  onStart() {    
  }

  stop() {
    if (!this.running) return;
    this.running = false;
    onStop();
  }

  onStop() {    
  }

}
