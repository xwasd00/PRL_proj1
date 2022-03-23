ifndef VERBOSE
.SILENT:
endif
PROJ=oems
SRC=$(PROJ).cpp
HDR=$(PROJ).h
TEST-FILE=test.sh
LOGIN=xsovam00
TMPDIR=tmp-prl
NUMBERS=8
PROCESSES=20

.PHONY:test

test: merlin
	ssh merlin "PATH=$$PATH:/usr/local/share/OpenMPI/bin && cd $(TMPDIR) && bash -e $(TEST-FILE)"
	ssh merlin "rm -rf $(TPDIR)"

run: $(PROJ) numbers
	mpirun -np $(PROCESSES) ./$(PROJ)

$(PROJ): $(SRC)
	mpic++ -o $(PROJ) $(SRC)

doc: clean $(LOGIN).pdf
	evince $(LOGIN).pdf &

merlin: pack
	ssh merlin "mkdir -p $(TMPDIR)"
	scp -q $(LOGIN).zip merlin:$(TMPDIR)
	ssh merlin "cd $(TMPDIR) && unzip -oqq $(LOGIN).zip"

pack: $(LOGIN).pdf
	zip -rq $(LOGIN).zip $(SRC) $(HDR) $(TEST-FILE) $(LOGIN).pdf

$(LOGIN).pdf: $(LOGIN).tex
	pdflatex $(LOGIN).tex 1>/dev/null

numbers:
	dd if=/dev/random bs=1 count=$(NUMBERS) of=numbers 2>/dev/null

clean:
	rm -rf $(PROJ) $(LOGIN).pdf *.zip *.aux *.log *.out *.fdb_latexmk *.fls *.synctex.gz numbers