## Test environments

* local R installation, R 4.0.2
* local R installation, R 3.6.3 on Windows
* ubuntu 16.04 (on travis-ci), R 4.0.2
* `rhub::check_for_cran()` (Windows/Linux r-release and r-devel)
* Winbuilder r-oldrelease, r-release and r-devel
  https://win-builder.r-project.org/D1Yiy10GA42S/00check.log
  https://win-builder.r-project.org/LYNBSoy2K28l/00check.log
  
## Notes

This update

* fixes a compile error (on r-oldrel-windows) noted on the CRAN builds by 
  activating a workaround for the gcc 4.9.3 toolchain

https://cran.r-project.org/web/checks/check_results_jefferis_at_gmail.com.html#dracor
https://www.r-project.org/nosvn/R.check/r-oldrel-windows-ix86+x86_64/dracor-00check.html

* removes some unnecessary source files from the Draco library, reducing the 
  compiled size of the package
* updates the README to reflect that package's CRAN status

## R CMD check results

0 errors | 0 warnings | 1 note

* checking CRAN incoming feasibility ... NOTE
Maintainer: ‘Gregory Jefferis <jefferis@gmail.com>’

Days since last update: 2

