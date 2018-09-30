Component = require '../common/component'
readline = require('readline')

module.exports = class KeyboardInput extends Component
  
  on_start: ->
    @current_map = {}
    console.log 'Starting Keyboard'
    readline.emitKeypressEvents(process.stdin)
    process.stdin.setRawMode(true)
    process.stdin.on 'keypress', @on_key.bind(@)

  on_key: (chunk, key) ->
    if key && key.name == 'c' and key.ctrl
      console.log("bye bye")
      process.exit()
    name = key.name
    name = "ctrl_#{name}" if key.ctrl
    @current_map[name] = 1

  flush: ->
    temp = @current_map
    @current_map = {}
    temp