#' Decode Draco encoded raw bytes containing mesh or point cloud data
#'
#' @param data \code{\link{raw}} bytes containing Draco data e.g. as read by
#'   \code{\link{readBin}}
#'
#' @return a list containing elements \code{points} and (for meshes)
#'   \code{faces}.
#' @export
#'
#' @examples
#'
#' \dontrun{
#' ## convert to standard rgl::mesh3d object
#' res=draco_decode(data)
#' m=rgl::tmesh3d(vertices=res$points, indices=res$faces, homogeneous = FALSE)
#' }
draco_decode <- function(data) {
  if(!is.raw(data))
    stop("Expecting raw data!")
  if(length(data)==0)
    stop("data contains 0 bytes!")

  res=.Call(`_dracor_dracodecode`, data)
  if(length(res)==0)
    stop("Unable to parse input data!")
  res
}

# Private utility function
draco_decodefile <- function(x) {
  data=readBin(x, what = raw(), n = file.info(x)$size)
  draco_decode(data)
}
