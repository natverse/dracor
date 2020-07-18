test_that("decoding works", {
  infile="testdata/mesh.draco"
  testthat::expect_known_value(draco_decodefile(infile), 'testdata/mesh.rds')
  # m2=Rvcg::vcgPlyRead('testdata/mesh.ply', clean = F, updateNormals = T)
  # m3=readobj::read.obj('testdata/mesh.obj')
})

# (base) Gregs-MBP-2:build jefferis$ ./draco_encoder -i /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.ply -o /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.ply.draco
# Encoder options:
#   Compression level = 7
# Positions: Quantization = 11 bits
#
# Encoded mesh saved to /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.ply.draco (7 ms to encode).
#
# Encoded size = 6074 bytes
#
# For better compression, increase the compression level up to '-cl 10' .
#
# (base) Gregs-MBP-2:build jefferis$ ./draco_encoder -i /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.obj -o /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.obj.draco
# Encoder options:
#   Compression level = 7
# Positions: Quantization = 11 bits
#
# Encoded mesh saved to /Users/jefferis/dev/R/dracor/tests/testthat/testdata/mesh.obj.draco (2 ms to encode).
#
# Encoded size = 6074 bytes
#
# For better compression, increase the compression level up to '-cl 10' .
test_that("draco round trip works", {
  origfile="testdata/mesh.draco"
  rtply="testdata/mesh.ply.draco"
})
