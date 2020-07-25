## Test environments
* local R installation, R 4.0.2
* ubuntu 16.04 (on travis-ci), R 4.0.2
* `rhub::check_for_cran()` (Windows/Linux r-release and r-devel)
* `rhub::check_for_solaris()`

## R CMD check results

0 errors | 0 warnings | 1 note

* This is a new submission.
* On solaris there was a second note:

* checking package dependencies ... NOTE
Package which this enhances but not available for checking: ‘rgl’

* and on some linux instances there was a note of the form:

* checking installed package size ... NOTE
  installed size is 17.5Mb
  sub-directories of 1Mb or more:
    libs  17.3Mb

otherwise no significant issues
