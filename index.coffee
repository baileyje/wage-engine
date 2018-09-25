Engine = require './engine'

engine = new Engine()
engine.state.objects.push { pos: [10, 10, 0] }
engine.start()

