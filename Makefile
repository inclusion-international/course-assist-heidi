build:
	latexmk -xelatex

preview:
	latexmk -pvc

clean:
	latexmk -c

distclean:
	latexmk -C