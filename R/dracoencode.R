#' Encode mesh to Draco mesh or point cloud data
#'
#' @param mesh a \code{rgl::mesh3d} object or a list containing elements
#'   \code{points} and (for meshes) \code{faces}.
#' @param file_name a chcaracter with output file name (suggested extension:
#' ".drc")
#'
#' @export
#'
#' @examples
#' \dontrun{
#'  draco_encode(mesh)
#' }
draco_encode <- function(mesh, file_name = "out.drc") {
  if (!grepl("\\.drc$", file_name))
    file_name <- paste0(file_name, ".drc")
  num_vertices <- dim(mesh$vb)[[2]]
  num_faces <- dim(mesh$it)[[2]]

  vertices <- as.vector(mesh$vb[1:3,])
  faces <- as.vector(mesh$it)
  dracoencode(vertices, faces, num_vertices, num_faces)
}
