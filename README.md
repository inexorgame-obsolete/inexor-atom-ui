# Sauerbraten Fork Atom UI

This contains an attempt at combining an SDL application
(our Sauerbraten Fork) and the Atom Shell/Node.js.

The life of this project will end if it has been merged with
our Sauerbraten fork.

## Structure

Our plan is to divide the jobs like this:

* Rendering/3D by SDL/C++
* Scripting with Node.js/Coffeescript
* GUI as a website with Coffescript/the atom-shell

## Why?

* Many people can code websites, many can code JS. Few can code C++, even fewer can code Sauerbraten.
* Coding JS is quicker & a lot smaller
* Package system for free (npm/apm)
* Implicit support for: opus, ogg, webm, TTF Fonts, OTF Fonts, webP
* Easy to implement a REST API
* If we're clever, we get an administrative interface for the server
* Tons of JS libs (e.g. JS window management, compression tools,
  web terminal, etherpad, bittorrent)

## War plan:

* **[DONE]** Create a basic atom app using coffescript
* **[DONE]** Write basic SDL app
* **[DONE]** Statically compile atom shell into our test
  app
* **[DONE]** Create a minimal testing API
* Integrate both SDL and atom shell in one window
* (Show of what the GUI could look like in future)
* Port to our sauerbraten fork
