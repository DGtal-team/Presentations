ALLJOBS= presDGTalFlux3D 

PSJOBS=$(ALLJOBS:=.ps)
REDJOBS=$(ALLJOBS:=.red.ps)
PDFJOBS=$(ALLJOBS:=.pdf)
FORJOBS=$(ALLJOBS:=.force)

PS2PDF_OPTIONS = -dPDFSETTINGS=/prepress -dUseFlateCompression=true -sColorImageFilter=FlateEncode -sGrayImageFilter=FlateEncode -dAutoFilterColorImages=false  -dAutoFilterGrayImages=false -dEncodeColorImages=false  -dEncodeGrayImages=false  -dEncodeMonoImages=false

all: $(PSJOBS)
force: $(FORJOBS)
pdf: $(PDFJOBS)
ps: $(PSJOBS)
red: $(REDJOBS)


pdf: $(ALLJOBS).pdf


%.dvi: %.tex 1intro.tex 2visu3D.tex 3futur.tex
	latex $<

%.ps: %.dvi
	dvips -Ppdf $< -o

%.pdf: %.ps
	ps2pdf $(PS2PDF_OPTIONS) $< $(<:.ps=.pdf)

%.force: %.tex
	latex $< && \
	bibtex $(<:.tex=) && \
	latex $< && \
	latex $< && \

	dvips -Ppdf $(<:.tex=.dvi) -o

%.red.ps: %.dvi
	dvips -Ppdf -ta4 $< -o $(<:.dvi=.temp.ps) && \
	psnup -8 -W128mm -H96mm -pa4 $(<:.dvi=.temp.ps) $(<:.dvi=.red.ps) && \
	rm -f $(<:.dvi=.temp.ps)

red.ps: 
	psnup -8 -W128mm -H96mm -pa4 $(<:.dvi=.tempp.ps) $(<:.dvi=.red.ps) && \
	rm -f $(<:.dvi=.temp.ps)




clean:
	echo "Cleaning temp files"
	rm -f  *.toc *.aux *.cp *.fn *.tp *.vr *.pg *.ky *.bbl *.blg *.tex~ *.bib~ *.out *.nav *.log *.snm


all: 








