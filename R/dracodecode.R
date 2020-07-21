#' Decode Draco encoded raw bytes containing mesh or point cloud data
#'
#' @param data \code{\link{raw}} bytes containing Draco data e.g. as read by
#'   \code{\link{readBin}}
#' @param mesh3d Whether to return \code{rgl::mesh3d} object (when \code{TRUE},
#'   the default) or something as close as possible to what is provided by the
#'   Draco library (when \code{FALSE}).
#'
#' @return a \code{rgl::mesh3d} object or a list containing elements
#'   \code{points} and (for meshes). \code{faces}.
#' @export
#' @details Note that the Draco library returns 0-based indices for the faces
#'   whereas R in general and \code{rgl::mesh3d} in particular expect 1-based
#'   indices. When \code{mesh3d=FALSE}, the result will have 0-based indices as
#'   returned by the Draco library.
#' @examples
#'
#' \dontrun{
#' # fetch test data
#' resp=httr::GET('https://github.com/google/draco/blob/master/testdata/car.drc?raw=true')
#' car.drc=httr::content(resp, 'raw')
#' car.m=draco_decode(car.drc)
#' # show the result
#' rgl::shade3d(car.m, col='red')
#'
#' ## demonstrate conversion of raw form to standard rgl::mesh3d object
#' car.raw=draco_decode(data, mesh3d=FALSE)
#' car.m2 = rgl::tmesh3d(
#'   vertices = res$points,
#'   indices = res$faces + 1,
#'   homogeneous = FALSE)
#' }
draco_decode <- function(data, mesh3d=TRUE) {
  if(!is.raw(data))
    stop("The `data` argument should contain `raw` bytes!")
  if(!is.logical(mesh3d))
    stop("The `mesh3d` argument must be TRUE or FALSE!")
  if(length(data)==0)
    stop("The `data` argument contains 0 bytes!")

  # in raw mode do as little as possible to the output
  index_offset=ifelse(mesh3d, 1L, 0L)

  res=.Call(`_dracor_dracodecode`, data, index_offset)
  if(length(res)==0)
    stop("Unable to parse input data!")
  if(isTRUE(mesh3d)) {
    res <- structure(list(vb = rbind(res$points, 1),
                          it = res$faces),
                     class = c("mesh3d", "shape3d"))
  }
  res
}

# Private utility function
draco_decodefile <- function(x, mesh3d=TRUE) {
  data=readBin(x, what = raw(), n = file.info(x)$size)
  draco_decode(data, mesh3d=mesh3d)
}
