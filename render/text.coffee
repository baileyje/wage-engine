Component = require '../common/component'

render_horz_line = (width, left_char, right_char) ->
  out = left_char
  for col in [0...width]
    out += '─'
  out += right_char
  out += '\n'
  out

class ScreenBuffer

  constructor: (@width, @height) ->
    @top = render_horz_line @width, '┌', '┐'
    @bottom = render_horz_line @width, '└', '┘'

    @clear()

  clear: ->
    @cells = []
    # for row in [0...@height]
    #   @cells[row] = []
    #   for col in [0...@width]
    #     @cells[row][col] = undefined

  text: ->
    out = '\x1Bc'
    out += @top
    for row in [0...@height]
      out += '│'
      for col in [0...@width]
        item = @cells[(row * @width) + col]
        unless item?
          out += ' '
          continue
        # TODO:  Should be item not just text
        out += item
      out += '│\n'
    out += @bottom
    out

module.exports = class SimplePhysicsEngine extends Component

  constructor: (config = {}) ->
    super()
    @width = config.width or 80
    @height = config.height or 20
    @buffer = new ScreenBuffer @width, @height

  render: (frame, state) ->
    @buffer.clear()
    # TODO: update buffer based on state
    for obj in state.objects
      if obj.pos[0] < @width and obj.pos[1] < @height
        idx = Math.trunc  (obj.pos[1] * @width) + obj.pos[0]
      # console.log Math.trunc(idx)
      @buffer.cells[idx] = 'X'
    process.stdout.write @buffer.text()
    await sleep 10

# JUNK
sleep = (ms) ->
  new Promise (resolve) ->
    setTimeout resolve, ms
  # start = new Date().getTime()
  # continue while new Date().getTime() - start < ms