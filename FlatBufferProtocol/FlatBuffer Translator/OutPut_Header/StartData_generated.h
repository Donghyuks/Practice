// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_STARTDATA_EATER_STARTDATA_H_
#define FLATBUFFERS_GENERATED_STARTDATA_EATER_STARTDATA_H_

#include "flatbuffers/flatbuffers.h"

namespace Eater {
namespace StartData {

struct Vec3;

struct EnemyPosition;
struct EnemyPositionBuilder;

struct ManaPosition;
struct ManaPositionBuilder;

struct GameData;
struct GameDataBuilder;

struct UserData;
struct UserDataBuilder;

struct MatchData;
struct MatchDataBuilder;

struct BaseData;
struct BaseDataBuilder;

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

struct EnemyPosition FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef EnemyPositionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INDEX = 4,
    VT_TYPE = 6,
    VT_POSITION = 8
  };
  uint16_t index() const {
    return GetField<uint16_t>(VT_INDEX, 0);
  }
  uint16_t type() const {
    return GetField<uint16_t>(VT_TYPE, 0);
  }
  const Eater::StartData::Vec3 *position() const {
    return GetStruct<const Eater::StartData::Vec3 *>(VT_POSITION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_INDEX) &&
           VerifyField<uint16_t>(verifier, VT_TYPE) &&
           VerifyField<Eater::StartData::Vec3>(verifier, VT_POSITION) &&
           verifier.EndTable();
  }
};

struct EnemyPositionBuilder {
  typedef EnemyPosition Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_index(uint16_t index) {
    fbb_.AddElement<uint16_t>(EnemyPosition::VT_INDEX, index, 0);
  }
  void add_type(uint16_t type) {
    fbb_.AddElement<uint16_t>(EnemyPosition::VT_TYPE, type, 0);
  }
  void add_position(const Eater::StartData::Vec3 *position) {
    fbb_.AddStruct(EnemyPosition::VT_POSITION, position);
  }
  explicit EnemyPositionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<EnemyPosition> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<EnemyPosition>(end);
    return o;
  }
};

inline flatbuffers::Offset<EnemyPosition> CreateEnemyPosition(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t index = 0,
    uint16_t type = 0,
    const Eater::StartData::Vec3 *position = 0) {
  EnemyPositionBuilder builder_(_fbb);
  builder_.add_position(position);
  builder_.add_type(type);
  builder_.add_index(index);
  return builder_.Finish();
}

struct ManaPosition FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ManaPositionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INDEX = 4,
    VT_POSITION = 6
  };
  uint16_t index() const {
    return GetField<uint16_t>(VT_INDEX, 0);
  }
  const Eater::StartData::Vec3 *position() const {
    return GetStruct<const Eater::StartData::Vec3 *>(VT_POSITION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_INDEX) &&
           VerifyField<Eater::StartData::Vec3>(verifier, VT_POSITION) &&
           verifier.EndTable();
  }
};

struct ManaPositionBuilder {
  typedef ManaPosition Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_index(uint16_t index) {
    fbb_.AddElement<uint16_t>(ManaPosition::VT_INDEX, index, 0);
  }
  void add_position(const Eater::StartData::Vec3 *position) {
    fbb_.AddStruct(ManaPosition::VT_POSITION, position);
  }
  explicit ManaPositionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ManaPosition> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ManaPosition>(end);
    return o;
  }
};

inline flatbuffers::Offset<ManaPosition> CreateManaPosition(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t index = 0,
    const Eater::StartData::Vec3 *position = 0) {
  ManaPositionBuilder builder_(_fbb);
  builder_.add_position(position);
  builder_.add_index(index);
  return builder_.Finish();
}

struct GameData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GameDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DAY = 4,
    VT_MANACOUNT = 6,
    VT_MANASPAWN = 8,
    VT_ENEMYSPAWN = 10
  };
  int8_t day() const {
    return GetField<int8_t>(VT_DAY, 0);
  }
  uint16_t manacount() const {
    return GetField<uint16_t>(VT_MANACOUNT, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::ManaPosition>> *manaspawn() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::ManaPosition>> *>(VT_MANASPAWN);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>> *enemyspawn() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>> *>(VT_ENEMYSPAWN);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_DAY) &&
           VerifyField<uint16_t>(verifier, VT_MANACOUNT) &&
           VerifyOffset(verifier, VT_MANASPAWN) &&
           verifier.VerifyVector(manaspawn()) &&
           verifier.VerifyVectorOfTables(manaspawn()) &&
           VerifyOffset(verifier, VT_ENEMYSPAWN) &&
           verifier.VerifyVector(enemyspawn()) &&
           verifier.VerifyVectorOfTables(enemyspawn()) &&
           verifier.EndTable();
  }
};

struct GameDataBuilder {
  typedef GameData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_day(int8_t day) {
    fbb_.AddElement<int8_t>(GameData::VT_DAY, day, 0);
  }
  void add_manacount(uint16_t manacount) {
    fbb_.AddElement<uint16_t>(GameData::VT_MANACOUNT, manacount, 0);
  }
  void add_manaspawn(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::ManaPosition>>> manaspawn) {
    fbb_.AddOffset(GameData::VT_MANASPAWN, manaspawn);
  }
  void add_enemyspawn(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>>> enemyspawn) {
    fbb_.AddOffset(GameData::VT_ENEMYSPAWN, enemyspawn);
  }
  explicit GameDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<GameData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GameData>(end);
    return o;
  }
};

inline flatbuffers::Offset<GameData> CreateGameData(
    flatbuffers::FlatBufferBuilder &_fbb,
    int8_t day = 0,
    uint16_t manacount = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::ManaPosition>>> manaspawn = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>>> enemyspawn = 0) {
  GameDataBuilder builder_(_fbb);
  builder_.add_enemyspawn(enemyspawn);
  builder_.add_manaspawn(manaspawn);
  builder_.add_manacount(manacount);
  builder_.add_day(day);
  return builder_.Finish();
}

inline flatbuffers::Offset<GameData> CreateGameDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int8_t day = 0,
    uint16_t manacount = 0,
    const std::vector<flatbuffers::Offset<Eater::StartData::ManaPosition>> *manaspawn = nullptr,
    const std::vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>> *enemyspawn = nullptr) {
  auto manaspawn__ = manaspawn ? _fbb.CreateVector<flatbuffers::Offset<Eater::StartData::ManaPosition>>(*manaspawn) : 0;
  auto enemyspawn__ = enemyspawn ? _fbb.CreateVector<flatbuffers::Offset<Eater::StartData::EnemyPosition>>(*enemyspawn) : 0;
  return Eater::StartData::CreateGameData(
      _fbb,
      day,
      manacount,
      manaspawn__,
      enemyspawn__);
}

struct UserData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UserDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4,
    VT_CHARACTER_TYPE = 6,
    VT_PLAYER_INDEX = 8,
    VT_POSITION = 10,
    VT_SPEED = 12
  };
  uint32_t key() const {
    return GetField<uint32_t>(VT_KEY, 0);
  }
  uint16_t character_type() const {
    return GetField<uint16_t>(VT_CHARACTER_TYPE, 0);
  }
  uint16_t player_index() const {
    return GetField<uint16_t>(VT_PLAYER_INDEX, 0);
  }
  const Eater::StartData::Vec3 *position() const {
    return GetStruct<const Eater::StartData::Vec3 *>(VT_POSITION);
  }
  float speed() const {
    return GetField<float>(VT_SPEED, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_KEY) &&
           VerifyField<uint16_t>(verifier, VT_CHARACTER_TYPE) &&
           VerifyField<uint16_t>(verifier, VT_PLAYER_INDEX) &&
           VerifyField<Eater::StartData::Vec3>(verifier, VT_POSITION) &&
           VerifyField<float>(verifier, VT_SPEED) &&
           verifier.EndTable();
  }
};

struct UserDataBuilder {
  typedef UserData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(uint32_t key) {
    fbb_.AddElement<uint32_t>(UserData::VT_KEY, key, 0);
  }
  void add_character_type(uint16_t character_type) {
    fbb_.AddElement<uint16_t>(UserData::VT_CHARACTER_TYPE, character_type, 0);
  }
  void add_player_index(uint16_t player_index) {
    fbb_.AddElement<uint16_t>(UserData::VT_PLAYER_INDEX, player_index, 0);
  }
  void add_position(const Eater::StartData::Vec3 *position) {
    fbb_.AddStruct(UserData::VT_POSITION, position);
  }
  void add_speed(float speed) {
    fbb_.AddElement<float>(UserData::VT_SPEED, speed, 0.0f);
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
    uint32_t key = 0,
    uint16_t character_type = 0,
    uint16_t player_index = 0,
    const Eater::StartData::Vec3 *position = 0,
    float speed = 0.0f) {
  UserDataBuilder builder_(_fbb);
  builder_.add_speed(speed);
  builder_.add_position(position);
  builder_.add_key(key);
  builder_.add_player_index(player_index);
  builder_.add_character_type(character_type);
  return builder_.Finish();
}

struct MatchData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MatchDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_USERS = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::UserData>> *users() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::UserData>> *>(VT_USERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_USERS) &&
           verifier.VerifyVector(users()) &&
           verifier.VerifyVectorOfTables(users()) &&
           verifier.EndTable();
  }
};

struct MatchDataBuilder {
  typedef MatchData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_users(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::UserData>>> users) {
    fbb_.AddOffset(MatchData::VT_USERS, users);
  }
  explicit MatchDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<MatchData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MatchData>(end);
    return o;
  }
};

inline flatbuffers::Offset<MatchData> CreateMatchData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::UserData>>> users = 0) {
  MatchDataBuilder builder_(_fbb);
  builder_.add_users(users);
  return builder_.Finish();
}

inline flatbuffers::Offset<MatchData> CreateMatchDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Eater::StartData::UserData>> *users = nullptr) {
  auto users__ = users ? _fbb.CreateVector<flatbuffers::Offset<Eater::StartData::UserData>>(*users) : 0;
  return Eater::StartData::CreateMatchData(
      _fbb,
      users__);
}

struct BaseData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BaseDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WORLD_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::GameData>> *world_data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::GameData>> *>(VT_WORLD_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_WORLD_DATA) &&
           verifier.VerifyVector(world_data()) &&
           verifier.VerifyVectorOfTables(world_data()) &&
           verifier.EndTable();
  }
};

struct BaseDataBuilder {
  typedef BaseData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_world_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::GameData>>> world_data) {
    fbb_.AddOffset(BaseData::VT_WORLD_DATA, world_data);
  }
  explicit BaseDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<BaseData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<BaseData>(end);
    return o;
  }
};

inline flatbuffers::Offset<BaseData> CreateBaseData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::StartData::GameData>>> world_data = 0) {
  BaseDataBuilder builder_(_fbb);
  builder_.add_world_data(world_data);
  return builder_.Finish();
}

inline flatbuffers::Offset<BaseData> CreateBaseDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Eater::StartData::GameData>> *world_data = nullptr) {
  auto world_data__ = world_data ? _fbb.CreateVector<flatbuffers::Offset<Eater::StartData::GameData>>(*world_data) : 0;
  return Eater::StartData::CreateBaseData(
      _fbb,
      world_data__);
}

}  // namespace StartData
}  // namespace Eater

#endif  // FLATBUFFERS_GENERATED_STARTDATA_EATER_STARTDATA_H_