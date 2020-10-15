## Test environments

* local R installation, R 4.0.3
* ubuntu 16.04 (on travis-ci), R 4.0.2
* Winbuilder r-devel


## Notes

This update

* fixes an error on gcc11 noted by BDR (see https://www.stats.ox.ac.uk/pub/bdr/gcc11/dracor.log) due to a misplaced include in the upstream draco library (see https://github.com/google/draco/issues/635)

## R CMD check results

0 errors | 0 warnings | 1 note

CRAN repository db overrides:
  X-CRAN-Comment: Archived on 2020-10-13 as check problems were not
    corrected in time.
