// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PLAYERDATA_EATER_GAMEDATA_H_
#define FLATBUFFERS_GENERATED_PLAYERDATA_EATER_GAMEDATA_H_

#include "flatbuffers/flatbuffers.h"

namespace Eater {
namespace GameData {

struct Vec3;

struct ClientStartInfo;
struct ClientStartInfoBuilder;

struct ClientMove;
struct ClientMoveBuilder;

struct WorldData;
struct WorldDataBuilder;

struct UserData;
struct UserDataBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3()
      : x_(0),
        y_(0),
        z_(0) {
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

struct ClientStartInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientStartInfoBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PLAYERINDEX = 4,
    VT_WORLD_SPEED = 6,
    VT_CHARACTER_TYPE = 8
  };
  uint16_t playerindex() const {
    return GetField<uint16_t>(VT_PLAYERINDEX, 0);
  }
  double world_speed() const {
    return GetField<double>(VT_WORLD_SPEED, 0.0);
  }
  uint16_t character_type() const {
    return GetField<uint16_t>(VT_CHARACTER_TYPE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_PLAYERINDEX) &&
           VerifyField<double>(verifier, VT_WORLD_SPEED) &&
           VerifyField<uint16_t>(verifier, VT_CHARACTER_TYPE) &&
           verifier.EndTable();
  }
};

struct ClientStartInfoBuilder {
  typedef ClientStartInfo Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_playerindex(uint16_t playerindex) {
    fbb_.AddElement<uint16_t>(ClientStartInfo::VT_PLAYERINDEX, playerindex, 0);
  }
  void add_world_speed(double world_speed) {
    fbb_.AddElement<double>(ClientStartInfo::VT_WORLD_SPEED, world_speed, 0.0);
  }
  void add_character_type(uint16_t character_type) {
    fbb_.AddElement<uint16_t>(ClientStartInfo::VT_CHARACTER_TYPE, character_type, 0);
  }
  explicit ClientStartInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientStartInfo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientStartInfo>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientStartInfo> CreateClientStartInfo(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t playerindex = 0,
    double world_speed = 0.0,
    uint16_t character_type = 0) {
  ClientStartInfoBuilder builder_(_fbb);
  builder_.add_world_speed(world_speed);
  builder_.add_character_type(character_type);
  builder_.add_playerindex(playerindex);
  return builder_.Finish();
}

struct ClientMove FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientMoveBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SEQUENCE = 4,
    VT_DTIME = 6,
    VT_MOV_VECTOR = 8
  };
  uint16_t sequence() const {
    return GetField<uint16_t>(VT_SEQUENCE, 0);
  }
  double dtime() const {
    return GetField<double>(VT_DTIME, 0.0);
  }
  const Eater::GameData::Vec3 *mov_vector() const {
    return GetStruct<const Eater::GameData::Vec3 *>(VT_MOV_VECTOR);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_SEQUENCE) &&
           VerifyField<double>(verifier, VT_DTIME) &&
           VerifyField<Eater::GameData::Vec3>(verifier, VT_MOV_VECTOR) &&
           verifier.EndTable();
  }
};

struct ClientMoveBuilder {
  typedef ClientMove Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_sequence(uint16_t sequence) {
    fbb_.AddElement<uint16_t>(ClientMove::VT_SEQUENCE, sequence, 0);
  }
  void add_dtime(double dtime) {
    fbb_.AddElement<double>(ClientMove::VT_DTIME, dtime, 0.0);
  }
  void add_mov_vector(const Eater::GameData::Vec3 *mov_vector) {
    fbb_.AddStruct(ClientMove::VT_MOV_VECTOR, mov_vector);
  }
  explicit ClientMoveBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientMove> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientMove>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientMove> CreateClientMove(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t sequence = 0,
    double dtime = 0.0,
    const Eater::GameData::Vec3 *mov_vector = 0) {
  ClientMoveBuilder builder_(_fbb);
  builder_.add_dtime(dtime);
  builder_.add_mov_vector(mov_vector);
  builder_.add_sequence(sequence);
  return builder_.Finish();
}

struct WorldData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef WorldDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_USERS = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Eater::GameData::UserData>> *users() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::GameData::UserData>> *>(VT_USERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_USERS) &&
           verifier.VerifyVector(users()) &&
           verifier.VerifyVectorOfTables(users()) &&
           verifier.EndTable();
  }
};

struct WorldDataBuilder {
  typedef WorldData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_users(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::GameData::UserData>>> users) {
    fbb_.AddOffset(WorldData::VT_USERS, users);
  }
  explicit WorldDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<WorldData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<WorldData>(end);
    return o;
  }
};

inline flatbuffers::Offset<WorldData> CreateWorldData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::GameData::UserData>>> users = 0) {
  WorldDataBuilder builder_(_fbb);
  builder_.add_users(users);
  return builder_.Finish();
}

inline flatbuffers::Offset<WorldData> CreateWorldDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Eater::GameData::UserData>> *users = nullptr) {
  auto users__ = users ? _fbb.CreateVector<flatbuffers::Offset<Eater::GameData::UserData>>(*users) : 0;
  return Eater::GameData::CreateWorldData(
      _fbb,
      users__);
}

struct UserData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UserDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PLAYERINDEX = 4,
    VT_ANIMATION = 6,
    VT_POSITION = 8,
    VT_SEQUENCE = 10
  };
  uint16_t playerindex() const {
    return GetField<uint16_t>(VT_PLAYERINDEX, 0);
  }
  uint16_t animation() const {
    return GetField<uint16_t>(VT_ANIMATION, 0);
  }
  const Eater::GameData::Vec3 *position() const {
    return GetStruct<const Eater::GameData::Vec3 *>(VT_POSITION);
  }
  uint16_t sequence() const {
    return GetField<uint16_t>(VT_SEQUENCE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_PLAYERINDEX) &&
           VerifyField<uint16_t>(verifier, VT_ANIMATION) &&
           VerifyField<Eater::GameData::Vec3>(verifier, VT_POSITION) &&
           VerifyField<uint16_t>(verifier, VT_SEQUENCE) &&
           verifier.EndTable();
  }
};

struct UserDataBuilder {
  typedef UserData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_playerindex(uint16_t playerindex) {
    fbb_.AddElement<uint16_t>(UserData::VT_PLAYERINDEX, playerindex, 0);
  }
  void add_animation(uint16_t animation) {
    fbb_.AddElement<uint16_t>(UserData::VT_ANIMATION, animation, 0);
  }
  void add_position(const Eater::GameData::Vec3 *position) {
    fbb_.AddStruct(UserData::VT_POSITION, position);
  }
  void add_sequence(uint16_t sequence) {
    fbb_.AddElement<uint16_t>(UserData::VT_SEQUENCE, sequence, 0);
  }
  explicit UserDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<UserData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UserData>(end);
    return o;
  }
};

inline flatbuffers::Offset<UserData> CreateUserData(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t playerindex = 0,
    uint16_t animation = 0,
    const Eater::GameData::Vec3 *position = 0,
    uint16_t sequence = 0) {
  UserDataBuilder builder_(_fbb);
  builder_.add_position(position);
  builder_.add_sequence(sequence);
  builder_.add_animation(animation);
  builder_.add_playerindex(playerindex);
  return builder_.Finish();
}

}  // namespace GameData
}  // namespace Eater

#endif  // FLATBUFFERS_GENERATED_PLAYERDATA_EATER_GAMEDATA_H_
