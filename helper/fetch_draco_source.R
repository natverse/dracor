# assumes that you have done
# git clone https://github.com/google/draco/
# cd draco
# mkdir build
# cd build
# cmake ..

dracodir="../../3d/draco"
files=scan(file.path(dracodir, "/build/CMakeFiles/dracodec.dir/link.txt"), what="")

ofiles=grep(".cc.o$", files, value = T)
other=grep(".cc.o$", files, value = T, invert = T)

ccfiles=sub(".cc.o$",".cc", ofiles)
ccfiles=sub(".*/src/","src/", ccfiles)

ccfiles=grep("CMakeFiles", ccfiles, value = T, invert = T)

inpath=file.path(dracodir, ccfiles)
stopifnot(all(file.exists(inpath)))

file.copy(inpath, 'src')

findhfiles <- function(cfile) {
  l=try(readLines(cfile), silent = TRUE)
  if(inherits(l, 'try-error')){
    message("unable to read file: ", cfile)
    return(character())
  }
  includes=grep('^#include "draco', l, value=T)
  hfiles=stringr::str_match(includes, '"([^"]+)"')[,2]
  hfiles
}

findhfiles.recursive <- function(infiles, hdir) {
  infiles=normalizePath(infiles)
  hdir=normalizePath(hdir)
  hfilel=sapply(infiles, findhfiles)
  hfiles=unique(unlist(hfilel))
  hfilepaths=file.path(hdir, hfiles)
  if(length(setdiff(hfilepaths, infiles))>0){
    message("Recursing!")
    hfilepaths=union(hfilepaths, findhfiles.recursive(hfilepaths, hdir=hdir))
  }
  return(hfilepaths)
}

allhfiles=findhfiles.recursive(file.path(dracodir,ccfiles),
                               hdir=file.path(dracodir,"src"))

allhfiles=grep("draco_features.h", allhfiles, value = T, invert = T)
stopifnot(all(file.exists(allhfiles)))

#special file
file.copy(file.path(dracodir, "build/draco/draco_features.h"), "src/draco")

srcdir=normalizePath(file.path(dracodir,"src"))
targetpaths=file.path('src', nat.utils::abs2rel(allhfiles, srcdir))
stopifnot(length(allhfiles)==length(targetpaths))
stopifnot(isTRUE(all.equal(basename(allhfiles), basename(targetpaths))))
targetsubdirs=unique(dirname(targetpaths))
sapply(targetsubdirs, dir.create, recursive = T)
mapply(file.copy, allhfiles, targetpaths, overwrite = T)

## now we still need to do some more fixes
# 1. for this check note
# File ‘dracor/libs/dracor.so’:
#   Found ‘___stderrp’, possibly from ‘stderr’ (C)
# Objects: ‘file_reader_factory.o’, ‘file_writer_factory.o’,
# ‘kd_tree_attributes_decoder.o’, ‘mesh_are_equivalent.o’,
# ‘stdio_file_reader.o’, ‘stdio_file_writer.o’
# Found ‘_printf’, possibly from ‘printf’ (C)
# Objects: ‘float_points_tree_decoder.o’,
# ‘kd_tree_attributes_decoder.o’

library(xfun)
xfun::gsub_dir("fprintf(stderr,", "REprintf(",
               dir = 'src', ext=c("cc", "h"), fixed=TRUE)

# also need to Rprintf
xfun::gsub_dir(" printf", " Rprintf",
               dir = 'src', ext=c("h"), fixed=TRUE)

# 2. paths longer that 100 chars
# dracor/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_transform_base.h

# plan is to replace prediction_scheme with ps in file/dir names and in source files

# this is >100
longestpath=file.path('dracor', targetpaths[which.max(nchar(targetpaths))])
nchar(longestpath)

# oof!
nchar(gsub("prediction_scheme", "ps", longestpath))

# OK so steps are
# 1. s/prediction_scheme/ps/ for all #include lines (but not other text)
# 2. s/prediction_scheme/ps/ for all file names
# 3. s/prediction_scheme/ps/ for all dir names

# repeat twice to do file and dir name of #include
for(i in 1:2)
  xfun::gsub_dir("(#include.*)prediction_scheme(.*)$", "\\1ps\\2",dir = 'src', ext = c("cc", "h"))

localhfiles=dir("src", patt='.*prediction_scheme.*\\.h$', recursive = TRUE, full.names = T)
renamedhfiles <- file.path(dirname(localhfiles), sub("prediction_scheme","ps", basename(localhfiles)))
mapply(file.rename, localhfiles, renamedhfiles)

hdirs <- dir("src/draco/", patt='prediction_scheme', include.dirs = T, full.names = T, recursive = T)
mapply(file.rename, hdirs, sub("prediction_scheme", "ps", hdirs))


xfun::gsub_file("src/draco/draco_features.h",
                "(#endif.*DRACO_FEATURES_H_)",
                "#ifdef __GNUC__\n#if __GNUC__ < 5\n#define DRACO_OLD_GCC\n#endif\n#endif\n\\1")
