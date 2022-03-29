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

test: 
	bash -e $(TEST-FILE)

run: $(PROJ) numbers
	mpirun -np $(PROCESSES) ./$(PROJ)

$(PROJ): $(SRC)
	mpic++ -o $(PROJ) $(SRC)

doc: clean $(LOGIN).pdf
	evince $(LOGIN).pdf &

merlin: pack
	ssh merlin "mkdir -p $(TMPDIR)"
	scp $(LOGIN).zip merlin:$(TMPDIR)
	ssh merlin "PATH=$$PATH:/usr/local/share/OpenMPI/bin && cd $(TMPDIR) && unzip -oqq $(LOGIN).zip && echo 'bash -e $(TEST-FILE)' && bash -e $(TEST-FILE)"
	ssh merlin "rm -rf $(TMPDIR)"

pack: $(LOGIN).pdf
	zip -r $(LOGIN).zip $(SRC) $(HDR) $(TEST-FILE) $(LOGIN).pdf

$(LOGIN).pdf: $(LOGIN).tex
	pdflatex $(LOGIN).tex

numbers:
	dd if=/dev/random bs=1 count=$(NUMBERS) of=numbers 2>/dev/null

clean:
	rm -rf $(PROJ) $(LOGIN).pdf *.zip *.aux *.log *.out *.fdb_latexmk *.fls *.synctex.gz numbers