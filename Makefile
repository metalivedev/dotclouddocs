# Makefile for Sphinx documentation
#

# You can set these variables from the command line.
export LC_ALL = en_US.UTF-8
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = _build
VERSIONS	  = 0.4 0.9

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) public

.PHONY: help clean html dirhtml pickle json htmlhelp qthelp latex changes linkcheck doctest figures

help:
	@echo "Please use \`make <target>' where <target> is one of"
#	@echo "  html      to make standalone HTML files"
#	@echo "  dirhtml   to make HTML files named index.html in directories"
#	@echo "  pickle    to make pickle files"
#	@echo "  json      to make JSON files"
#	@echo "  htmlhelp  to make HTML files and a HTML help project"
#	@echo "  qthelp    to make HTML files and a qthelp project"
	@echo "  latex     to make LaTeX files, you can set PAPER=a4 or PAPER=letter"
#	@echo "  changes   to make an overview of all changed/added/deprecated items"
#	@echo "  linkcheck to check all external links for integrity"
#	@echo "  doctest   to run all doctests embedded in the documentation (if enabled)"
#	@echo "  figures   to compile figures with ditaa"
#	@echo "  html      to build all the documentation locally"
	@echo "  docs      to build the public docs and upload to docs.dotcloud.com"
	@echo "  pdf       to build a PDF of the whole doc"

clean:
	-rm -rf $(BUILDDIR)/*
	-rm *.pyc

docs: $(VERSIONS)
	cp -a ./newdocs.www/. $(BUILDDIR)/newdocs.www

	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/newdocs.www."
	@echo "To see them:"
	@echo "( cd $(BUILDDIR)/newdocs.www ; exec python -m SimpleHTTPServer ) & firefox http://localhost:8000/ ; fg %+"
	@echo "or (on the platform@dotcloud.com dotCloud account):"
	@echo "dotcloud push docs $(BUILDDIR)/newdocs.www"

$(VERSIONS):
	@echo "Building $@"
	mkdir -p $(BUILDDIR)/newdocs.www/$@
	$(SPHINXBUILD)							\
		-D html_theme=structure					\
		-D html_static_path=					\
		-b dirhtml public/$@ $(BUILDDIR)/newdocs.www/$@

pdf: latex
	make -C $(BUILDDIR)/latex all-pdf

figures: architecture.png model.png incoherency-example.png

%.png: %.aa
	ditaa $< $@ -o -s 2 -r

coverage:
	$(SPHINXBUILD) -b coverage $(ALLSPHINXOPTS) $(BUILDDIR)/coverage
	@echo
	@echo "Build finished. The coverage information is in $(BUILDDIR)/coverage."

html:
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."
	@echo "To see them:"
	@echo "firefox file://$(shell pwd)/$(BUILDDIR)/html/index.html"

dirhtml:
	$(SPHINXBUILD) -b dirhtml $(ALLSPHINXOPTS) $(BUILDDIR)/dirhtml
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/dirhtml."

pickle:
	$(SPHINXBUILD) -b pickle $(ALLSPHINXOPTS) $(BUILDDIR)/pickle
	@echo
	@echo "Build finished; now you can process the pickle files."

json:
	$(SPHINXBUILD) -b json $(ALLSPHINXOPTS) $(BUILDDIR)/json
	@echo
	@echo "Build finished; now you can process the JSON files."

htmlhelp:
	$(SPHINXBUILD) -b htmlhelp $(ALLSPHINXOPTS) $(BUILDDIR)/htmlhelp
	@echo
	@echo "Build finished; now you can run HTML Help Workshop with the" \
	      ".hhp project file in $(BUILDDIR)/htmlhelp."

qthelp:
	$(SPHINXBUILD) -b qthelp $(ALLSPHINXOPTS) $(BUILDDIR)/qthelp
	@echo
	@echo "Build finished; now you can run "qcollectiongenerator" with the" \
	      ".qhcp project file in $(BUILDDIR)/qthelp, like this:"
	@echo "# qcollectiongenerator $(BUILDDIR)/qthelp/OFIS.qhcp"
	@echo "To view the help file:"
	@echo "# assistant -collectionFile $(BUILDDIR)/qthelp/OFIS.qhc"

latex:
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo
	@echo "Build finished; the LaTeX files are in $(BUILDDIR)/latex."
	@echo "Run \`make all-pdf' or \`make all-ps' in that directory to" \
	      "run these through (pdf)latex."

changes:
	$(SPHINXBUILD) -b changes $(ALLSPHINXOPTS) $(BUILDDIR)/changes
	@echo
	@echo "The overview file is in $(BUILDDIR)/changes."

linkcheck:
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/output.txt."

doctest:
	$(SPHINXBUILD) -b doctest $(ALLSPHINXOPTS) $(BUILDDIR)/doctest
	@echo "Testing of doctests in the sources finished, look at the " \
	      "results in $(BUILDDIR)/doctest/output.txt."
