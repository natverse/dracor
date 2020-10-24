## Test environments

* local R installation, R 4.0.3
* ubuntu 16.04 (on travis-ci), R 4.0.2
* Winbuilder r-devel
* R+gcc11 installation, R 4.0.3
  via https://github.com/r-hub/rhub/issues/425

https://win-builder.r-project.org/O0z88rt0Wxex/00check.log

## Notes

This update

* fixes two errors on gcc11 noted by BDR (see https://www.stats.ox.ac.uk/pub/bdr/gcc11/dracor.log) due to misplaced/missing includes in the upstream draco library (see https://github.com/google/draco/issues/635)
* note that this replaces dracor 0.2.3 which was submitted to CRAN but 
  Uwe Ligges responded indicating that there was still a missing header error
  on gcc 11

## R CMD check results

0 errors | 0 warnings | 1 note

CRAN repository db overrides:
  X-CRAN-Comment: Archived on 2020-10-13 as check problems were not
    corrected in time.
