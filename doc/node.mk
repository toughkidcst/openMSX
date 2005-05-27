# $Id$

include build/node-start.mk

SUBDIRS:=manual

# Not all of the docs are useful for end users, either because they are about
# the implementation, or because they are too rough/unfinished.
# Below is the list of docs that should be included in installation.
INSTALL_DOCS:= \
	release-notes.txt release-history.txt \
	after-bussum-FAQ.txt commands.txt exampleconfigs.xml \
	Using-diskmanipulator.txt

DIST:=$(INSTALL_DOCS) \
	debugdevice.txt \
	developersFAQ.txt testcases.txt \
	vram-addressing.txt \
	openmsx.tex MSX-cassette.dia \
	msxinfo-article.html schema1.png schema2.png screenshot.png

include build/node-end.mk
