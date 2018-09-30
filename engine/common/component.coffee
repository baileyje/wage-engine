
module.exports = class Component

  start: (@engine) ->
    return if @running
    @running = true
    @on_start()

  on_start: ->

  stop: ->
    return if !@running
    @running = false
    @on_stop()

  on_stop: ->
    