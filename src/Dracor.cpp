
#include <Rcpp.h>
using namespace Rcpp;
using namespace std;
#include <cinttypes>
#include "draco/draco_features.h"
#include "draco/compression/decode.h"
#include "draco/io/file_utils.h"

//' @export
// [[Rcpp::export]]
List dracodecodefile(CharacterVector x) {
  draco::DecoderBuffer buffer;

  std::vector<std::string> f = as<std::vector<std::string> >(x);
  std::vector<char> data;
  if (!draco::ReadFileToBuffer(f[0], &data)) {
    return List();
  }

  buffer.Init(data.data(), data.size());

  std::unique_ptr<draco::PointCloud> pc;
  draco::Mesh *mesh = nullptr;
  auto type_statusor = draco::Decoder::GetEncodedGeometryType(&buffer);
  if (!type_statusor.ok()) {
    return List();
  }
  const draco::EncodedGeometryType geom_type = type_statusor.value();

  if (geom_type == draco::TRIANGULAR_MESH) {
    draco::Decoder decoder;
    auto statusor = decoder.DecodeMeshFromBuffer(&buffer);
    if (!statusor.ok()) {
      return List();
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
      return List();
    }
    pc = std::move(statusor).value();
  }

  if (pc == nullptr) {
    // printf("Failed to decode the input file.\n");
    return List();
  }
  return List("success");
}

//' @export
// [[Rcpp::export]]
List dracodecode(RawVector data) {
  draco::DecoderBuffer buffer;
  buffer.Init((char*) &data[0], data.size());
  std::unique_ptr<draco::PointCloud> pc;
  draco::Mesh *mesh = nullptr;
  auto type_statusor = draco::Decoder::GetEncodedGeometryType(&buffer);
  if (!type_statusor.ok()) {
    return List();
  }
  const draco::EncodedGeometryType geom_type = type_statusor.value();

  if (geom_type == draco::TRIANGULAR_MESH) {
    draco::Decoder decoder;
    auto statusor = decoder.DecodeMeshFromBuffer(&buffer);
    if (!statusor.ok()) {
      return List();
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
      return List();
    }
    pc = std::move(statusor).value();
  }

  if (pc == nullptr) {
    // printf("Failed to decode the input file.\n");
    return List();
  }
  return List("success");
}
