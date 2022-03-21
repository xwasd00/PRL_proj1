MPICC=mpic++
PREFIX=--prefix /usr/local/share/OpenMPI
SRC=*.cpp
PROJ=oems
TEST-FILE=test.sh
LOGIN=xsovam00

.PHONY:test

test:
	sh -e $(TEST-FILE) $(NUMBERS)

$(PROJ): $(SRC)
	$(MPICC) -o $(PROJ) $(SRC)

doc: $(LOGIN).pdf clean-tex
	evince $(LOGIN).pdf &

pack: $(LOGIN).pdf
	zip -r $(LOGIN).zip $(SRC) $(TEST-FILE) $(LOGIN).pdf

$(LOGIN).pdf: $(LOGIN).tex
	pdflatex $(LOGIN).tex

clean-all: clean-tex
	rm -rf $(PROJ) $(LOGIN).pdf *.zip

clean-tex:
	rm -rf *.aux *.log *.out