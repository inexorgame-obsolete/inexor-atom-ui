jQuery = $ = require "jquery"
L = require "lazy.js"
require 'coffeecup'
cc = coffeecup # Damn you

head = (a) -> a[0]
tail = (a) -> a[1..]

P = (a...) ->
  console.log a...
  head a

# Iterate over elements
$each = (sel, f) ->
  _.map (_.map ($ sel), $),   f

# Alter (MAP) elements
$map = (sel, f) ->
  $each sel, (e, a...) ->
    e.replaceWith f e.clone(), a...

rev = (a) -> a.reverse()

# Higher order function;
# Reverse the arguments of a function
frev = (f) ->
  (a...) ->
    f (rev a)...

split = (s, c) -> s.split c
join  = (v, c) -> v.join c

ckr = frev cc.render

millis =-> new Date().getTime()
unixt  =-> millis(date = millis()) / 1000

jQuery.fn.id = (name) ->
  @attr 'id', name

doc  = $ document
win  = $ window
body = null

mkdiv = -> $ ckr -> div()

delay = frev setTimeout
############################################################

addLayer = (name, html="") ->
  l = mkdiv()
  l.addClass "layer"
  l.id name
  l.html html
  body.append l
  $ ".layer#" + name


doc.ready ->
  body = $ "body"
  bg = addLayer "bg"
  bg.css "background-size", "100% 100%"
  bg.css "background", "url('bg.webp')"


  return

  bg = addLayer "GL", ckr ->
    canvas style: "width: 100%; height: 100%"

  can = head $ 'canvas'

  gl = can.getContext "webgl"
  gl.viewportWidth = can.width
  gl.viewportHeight = can.height
  gl.clearColor(0.9, 0.6, 0.3, 0.9)
  gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight)
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

  #buf = []
  #buf.tri_vertex = gl.createBuffer()
  #gl.bindBuffer gl.ARRAY_BUFFER, buf.tri_vertex
  #vertices = [
       #0.0,  1.0,  0.0,
      #-1.0, -1.0,  0.0,
       #1.0, -1.0,  0.0
  #]
  #buf.tri_vertex.itemSize = 3
  #buf.tri_vertex.numItems = 3

  
  #buf.squ_vertex = gl.createBuffer()
  #gl.bindBuffer(gl.ARRAY_BUFFER, buf.squ_vertex)
  #vertices = [
       #1.0,  1.0,  0.0,
      #-1.0,  1.0,  0.0,
       #1.0, -1.0,  0.0,
      #-1.0, -1.0,  0.0
  #]
  #gl.bufferData gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW
  #buf.squ_vertex.itemSize = 3
  #buf.squ_vertex.numItems = 4



