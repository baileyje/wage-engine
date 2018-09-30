Component = require '../common/component'

module.exports = class SimplePhysicsEngine extends Component

  constructor: (config) ->
    super()

  simulate: (frame, state) ->
    # console.log 'Simulate: ', frame, state
    for obj in state.objects
      if state.input.keyboard.right?
        obj.pos[0] += 1
      if state.input.keyboard.left?
        obj.pos[0] -= 1
      if state.input.keyboard.up?
        obj.pos[1] -= 1
      if state.input.keyboard.down?
        obj.pos[1] += 1
    sleep 6




# JUNK
sleep = (ms) ->
  start = new Date().getTime()
  continue while new Date().getTime() - start < ms