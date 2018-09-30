now = ->
  # time = process.hrtime()
  # (time[0] * 1e9 + time[1]) / 1e9
  new Date().getTime()
  
Component = require '../common/component'

module.exports = class BasicGameLoop extends Component

    constructor: ->
      super()

    on_start: ->
      time = 0.0
      dt =  16
      previous_time = now()
      accumulator = 0.0
      while @running
        frame = { delta_time: dt }
        current_time = now()
        elapsed = current_time - previous_time
        previous_time = current_time
        accumulator += elapsed
        while accumulator >= dt
          await @engine.process_input()
          frame.time = time
          await @engine.simulate frame
          time += dt
          accumulator = accumulator - dt
        await @engine.render(frame)