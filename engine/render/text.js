class ScreenBuffer{
  
  constructor(width, height) {
    this.width = width;
    this.height = height;
    this.clear()
  }

  clear() {
    this.cells = [];
  }

  render(entities) {
    this.clear();
    entities.forEach(entity => {
      const baseIndex = Math.floor(entity.position.y) * this.width + Math.floor(entity.position.x);
      for (var row = 0; row < entity.textCells.length; row++) {
        const cols = entity.textCells[row];
        for (var col = 0; col < cols.length; col++) {
          const effectiveIndex = baseIndex + row * this.width + col;
          const text = entity.textCells[row][col];
          this.cells[effectiveIndex] = text;
        }
      }
      
    });
  }
}


class TextRenderer {

  constructor(entities, config = {}) {
    this.entities = entities;
    this.width = 80;
    this.height = 40;
    this.topLine = this.renderHorzLine(this.width,'┌', '┐');
    this.bottomLine = this.renderHorzLine(this.width, '└', '┘'); 
    this.screenBuffer = new ScreenBuffer(this.width, this.height);
  }

  init() {    
    console.log('Initializing Text Renderer');
  }

  start() {
  }

  renderHorzLine (width, leftChar, rightChar) {
    var out = leftChar;
    for (var col = 0; col < width; col++) {
      out += '─';
    }
    out += rightChar;
    out += '\n';
    return out;
  }

  update(ctx) {
    // var out = '\x1Bc';
    // out += this.topLine;
    // for (var row = 0; row < this.height; row++) {
    //   out += '│'
    //   for (var col = 0; col < this.width; col++) {
    //     out += ' ';
    //   }
    //   out += '│\n'
    // }
    // out += this.bottomLine;
    // console.log(out);
  }

  fixedUpdate(ctx) {
    // TODO - Should be update.
    this.screenBuffer.render(this.entities);
    var out = '\x1Bc';
    out += this.topLine;
    for (var row = 0; row < this.height; row++) {
      out += '│'
      for (var col = 0; col < this.width; col++) {
        var cellIndex = row * this.width + col;
        var text = this.screenBuffer.cells[cellIndex];
        if (text != undefined) {
          out += text;
        } else {
          out += ' ';
        }        
      }
      out += '│\n'
    }
    out += this.bottomLine;
    out += this.entities[0].position.x + ':' + this.entities[0].position.y + '\n';    
    console.log(out);
    
  }

  stop() {
  }

  deinit() {
    console.log('Deinitializing Text Renderer');
  }

}

export default TextRenderer;



// Component = require '../common/component'

// render_horz_line = (width, left_char, right_char) ->
//   out = left_char
//   for col in [0...width]
//     out += '─'
//   out += right_char
//   out += '\n'
//   out

// class ScreenBuffer

//   constructor: (@width, @height) ->
//     @top = render_horz_line @width, '┌', '┐'
//     @bottom = render_horz_line @width, '└', '┘'

//     @clear()

//   clear: ->
//     @cells = []
//     # for row in [0...@height]
//     #   @cells[row] = []
//     #   for col in [0...@width]
//     #     @cells[row][col] = undefined

//   text: ->
//     out = '\x1Bc'
//     out += @top
//     for row in [0...@height]
//       out += '│'
//       for col in [0...@width]
//         item = @cells[(row * @width) + col]
//         unless item?
//           out += ' '
//           continue
//         # TODO:  Should be item not just text
//         out += item
//       out += '│\n'
//     out += @bottom
//     out

// module.exports = class SimplePhysicsEngine extends Component

//   constructor: (config = {}) ->
//     super()
//     @width = config.width or 80
//     @height = config.height or 20
//     @buffer = new ScreenBuffer @width, @height

//   render: (frame, state) ->
//     @buffer.clear()
//     # TODO: update buffer based on state
//     for obj in state.objects
//       if obj.pos[0] < @width and obj.pos[1] < @height
//         idx = Math.trunc  (obj.pos[1] * @width) + obj.pos[0]
//       # console.log Math.trunc(idx)
//       @buffer.cells[idx] = 'X'
//     process.stdout.write @buffer.text()
//     await sleep 10

// # JUNK
// sleep = (ms) ->
//   new Promise (resolve) ->
//     setTimeout resolve, ms
//   # start = new Date().getTime()
//   # continue while new Date().getTime() - start < ms