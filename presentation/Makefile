all: presentation.tex
	latexmk -pdf -lualatex -shell-escape presentation.tex

clean:
	$(RM) -rf _minted-presentation
	$(RM) presentation.nav presentation.snm
	latexmk -C presentation.tex

.PHONY: all clean
