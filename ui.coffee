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

############################################################

addLayer = (name, html="") ->
  l = mkdiv()
  l.addClass "layer"
  l.id name
  body.append l
  $ ".layer#" + name


doc.ready ->
  body = $ "body"
  bg = addLayer "bg"
  bg.css "background", "url('bg.webp')"
  bg.css "background-size", "100% 100%"
