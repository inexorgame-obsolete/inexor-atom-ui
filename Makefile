# This is to be replaced by a proper node module
pull-deps:
	mkdir -p vendor
	test -e vendor/ventus || svn co https://github.com/sauerbraten-fork/Ventus/trunk/build vendor/ventus

