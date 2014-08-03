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
  #bg = addLayer "bg"
  #bg.css "background-position: 50% 50%"
  #bg.css "background-size", "20% 20%"
  #bg.css "background-repeat", "no-repeat"
  #bg.css "background", "url('bg.webp')"


  return
