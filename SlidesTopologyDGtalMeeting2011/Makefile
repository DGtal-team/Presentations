MAIN_TEX=slides.tex


all: ${MAIN_TEX}
	export TEXINPUTS=".:./common:"; pdflatex ${MAIN_TEX}

fig:
	cd Figures; make all

fullclean: clean
	rm -f *~

clean:
	rm -f *.aux *.log *.bbl *.blg *.ps *.dvi *.pdf *.toc *.out *.snm *.nav *.vrb
	cd Figures; make clean
