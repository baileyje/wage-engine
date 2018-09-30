DefaultGameLoop = require './loop/basic'
SimplePhysicsEngine = require './phys/simple'
TextRenderEngine = require './render/text'
DefaultInputManager = require './input/manager'


###
Game Engine

- Primary interface into into the system.  Manages the the game state and underlying systems.

###
module.exports = class Engine

  state: {
    objects: []
  }


  constructor: (config = {}) ->
    @loop = config.loop or new DefaultGameLoop()
    @phys = config.phys or new SimplePhysicsEngine()
    @renderer = config.renderer or new TextRenderEngine()
    @input_manager = config.input_manager or new DefaultInputManager()

  ###
    Start the enging and starts the game loop.
  ###
  start: ->
    return if @running
    console.log 'Starting NGE'
    @running = true
    @phys.start @
    @renderer.start @
    @input_manager.start @
    console.log 'Loop!'
    # LAST
    @loop.start @
    
    
  ###
    Stops the game loop and shutdown the engine.
  ###
  stop: ->
    console.log 'Stopping NGE'
    @loop.stop()
    @running = false

  ###
    Processes user input and collects it into the frame state.
  ###
  process_input: ->
    # console.log 'Input'
    @input_manager.process @state
    

  ###
    Run the world simulation and upates the game state.  Invokes the physics service.
  ###
  simulate: (frame) ->
    @phys.simulate frame, @state
    
  ###
    Render the world.  Invokes the render service.
  ###
  render: (frame) ->
    await @renderer.render frame, @state
