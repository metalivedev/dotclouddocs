dotCloud Platform Documentation
===============================
This project creates the dotCloud Platform documentation. 
Copyright dotCloud 2013

The official and published location of these documents is http://docs.dotcloud.com

**dotCloud welcomes pull requests to fix documentation issues!**

Any changes to the dotCloud documentation that we make based on pull requests become copyrighted by dotCloud.

Creating the published form of the docs requires installing Sphinx (http://sphinx-doc.org/)

```
$ make docs
Building 0.4
mkdir -p _build/newdocs.www/0.4
sphinx-build  						\
		-D html_theme=structure					\
		-D html_static_path=					\
		-b dirhtml public/0.4 _build/newdocs.www/0.4
Running Sphinx v1.1.3
loading pickled environment... done
building [dirhtml]: targets for 0 source files that are out of date
updating environment: 0 added, 0 changed, 0 removed
looking for now-outdated files... none found
no targets are out of date.
Building 0.9
mkdir -p _build/newdocs.www/0.9
sphinx-build							\
		-D html_theme=structure					\
		-D html_static_path=					\
		-b dirhtml public/0.9 _build/newdocs.www/0.9
Running Sphinx v1.1.3
loading pickled environment... done
building [dirhtml]: targets for 0 source files that are out of date
updating environment: 0 added, 0 changed, 0 removed
looking for now-outdated files... none found
no targets are out of date.
cp -a ./newdocs.www/. _build/newdocs.www

Build finished. The HTML pages are in _build/newdocs.www.
To see them:
( cd _build/newdocs.www ; exec python -m SimpleHTTPServer ) & firefox http://localhost:8000/ ; fg %+
```

The build results from Sphinx are in `_build/newdocs.www`.

A simple way to serve the build results (if you don't have a web server on your build machine) is
with `python -m SimpleHTTPServer` (as above).
