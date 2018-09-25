
process.stdin.setRawMode(true)
process.stdin.resume()
process.stdin.on 'data', (key) ->
  console.log key.toString()
