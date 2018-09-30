Component = require '../common/component'
KeyboardInput = require './keyboard'

module.exports = class InputManager extends Component

  constructor: ->
    super()
    @keyboard = new KeyboardInput()

  on_start: ->
    console.log 'Starting Input Manager'
    @keyboard.start()

  process: (state) ->
    state.input =
      keyboard: @keyboard.flush()
    # Hack to allow async    
    new Promise (resolve) ->
      setTimeout resolve, 1