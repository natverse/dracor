
#include <Rcpp.h>
using namespace Rcpp;
using namespace std;
#include <cinttypes>
#include "draco/draco_features.h"
#include "draco/compression/decode.h"
#include "draco/io/ply_decoder.h"

#include "draco/mesh/mesh.h"
#include "draco/point_cloud/point_cloud.h"
#include "draco/attributes/point_attribute.h"

#include "draco/core/encoder_buffer.h"
#include "draco/compression/encode.h"
#include "draco/io/mesh_io.h"
#include "draco/io/file_utils.h"

//
// List dracodecodefile(CharacterVector x) {
//   draco::DecoderBuffer buffer;
//
//   std::vector<std::string> f = as<std::vector<std::string> >(x);
//   std::vector<char> data;
//   if (!draco::ReadFileToBuffer(f[0], &data)) {
//     return List("Failed to read file");
//   }
//
//   buffer.Init(data.data(), data.size());
//   return List("Not implemented");
// }

// [[Rcpp::export]]
List dracodecode(RawVector data, const int index_offset=1) {
  draco::DecoderBuffer buffer;
  buffer.Init((char*) &data[0], data.size());
  std::unique_ptr<draco::PointCloud> pc;
  draco::Mesh *mesh = nullptr;
  auto type_statusor = draco::Decoder::GetEncodedGeometryType(&buffer);

  if (!type_statusor.ok()) {
    return List("Unable to determine geometry. Bad input data?");
  }
  const draco::EncodedGeometryType geom_type = type_statusor.value();

  if (geom_type == draco::TRIANGULAR_MESH) {
    draco::Decoder decoder;
    auto statusor = decoder.DecodeMeshFromBuffer(&buffer);
    if (!statusor.ok()) {
      return List("Unable to decode triangular mesh data");
    }
    std::unique_ptr<draco::Mesh> in_mesh = std::move(statusor).value();
    if (in_mesh) {
      mesh = in_mesh.get();
      pc = std::move(in_mesh);
    }
  } else if (geom_type == draco::POINT_CLOUD) {
    draco::Decoder decoder;
    auto statusor = decoder.DecodePointCloudFromBuffer(&buffer);
    if (!statusor.ok()) {
      return List("Unable to decode point cloud data");
    }
    pc = std::move(statusor).value();
  } else {
    return List("Unsupported geometry type");
  }

  if (pc == nullptr) {
    return List("Failed to decode the input data");
  }

  // get vertex positions
  const draco::PointAttribute *const att =
    pc->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  if (att == nullptr || att->size() == 0) {
    return List("No 3D position attribute found in data");
  }
  std::array<float, 3> vertex;
  const uint32_t nverts = static_cast<uint32_t>(att->size());

  NumericMatrix verts(3, nverts);
  // second loop counter
  uint32_t ii=0;
  for (draco::AttributeValueIndex i(0); i < nverts; ++i) {
    if (!att->ConvertValue<float, 3>(i, &vertex[0])) {
      return List("Error converting 3D vertex positions");
    }
    for (int j = 0; j < 3; j++) {
      verts(j, ii)=vertex[j];
    }
    ++ii;
  }
  List ret = List::create(Named("points")=verts);

  if(geom_type == draco::TRIANGULAR_MESH) {
    IntegerMatrix faceindices(3, mesh->num_faces());
    // get indices for faces
    ii=0;
    for (draco::FaceIndex i(0); i < mesh->num_faces(); ++i) {
      for (int j = 0; j < 3; ++j) {
        draco::PointIndex vert_index = mesh->face(i)[j];
        faceindices(j, ii)=att->mapped_index(vert_index).value() + index_offset;
      }
      ++ii;
    }

    ret["faces"]=faceindices;
  }

  return ret;
}

int EncodeMeshToFile(const draco::Mesh &mesh, const std::string &file,
                     draco::Encoder *encoder) {
  // Encode the geometry.
  draco::EncoderBuffer buffer;
  /*
  const draco::Status status = encoder->EncodeMeshToBuffer(mesh, &buffer);
  if (!status.ok()) {
    return -1;//List("Failed to encode mesh.");
  }
  // Save the encoded geometry into a file.
  if (!draco::WriteBufferToFile(buffer.data(), buffer.size(), file)) {
    return -1;//List("Failed to create the output file.");
  }
  */
  return 0;//List("ok")
}

// [[Rcpp::export]]
void dracoencode(const NumericVector vertices, const NumericVector faces,
                 int num_vertices, int num_faces) {
  // create draco mesh
  draco::Mesh mesh;

  // add vertices
  draco::GeometryAttribute va;
  va.Init(draco::GeometryAttribute::NORMAL, nullptr, 3, draco::DT_FLOAT32, false,
          sizeof(float) * 3, 0);
  const int att_id = mesh.AddAttribute(va, true, num_vertices);

  int idx;
  std::array<float, 3> val;
  for (draco::PointIndex::ValueType i = 0; i < num_vertices; ++i) {
    idx = (int) i;
    val[0] = vertices[3*idx];
    val[1] = vertices[3*idx+1];
    val[2] = vertices[3*idx+2];
    mesh.attribute(att_id)->SetAttributeValue(draco::AttributeValueIndex(i), &val[0]);
  }
  //faces
  std::array<draco::PointIndex, 3> ff;
  for (int i = 0; i < num_faces; ++i) {
    ff = {draco::PointIndex(faces[3*i]),
          draco::PointIndex(faces[3*i+1]),
          draco::PointIndex(faces[3*i+2])};
    mesh.AddFace(ff);
  }

  Rprintf("Creating Draco Mesh Done\n");

  // encode draco mesh
  draco::Encoder encoder;
  // so far hardcoded, to be parametrized later
  int pos_quantization_bits = 11;
  int compression_level = 2;
  const int speed = 10 - compression_level;
  /*
  std::string output = "out.drc";

  encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, pos_quantization_bits);
  encoder.SetSpeedOptions(speed, speed);
  int ret;
  ret = EncodeMeshToFile(mesh, output, &encoder);
  if (ret != -1)
    Rprintf("Encoding correct.\n");
  else
    Rprintf("Problem with encoding!\n");
  */
}
