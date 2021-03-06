// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LOGINLAUNCHER_EATER_LOGINLAUNCHER_H_
#define FLATBUFFERS_GENERATED_LOGINLAUNCHER_EATER_LOGINLAUNCHER_H_

#include "flatbuffers/flatbuffers.h"

namespace Eater {
namespace LoginLauncher {

struct UserState;
struct UserStateBuilder;

struct LoginReqData;
struct LoginReqDataBuilder;

struct LoginResData;
struct LoginResDataBuilder;

struct RealTimeData;
struct RealTimeDataBuilder;

struct AddFriend;
struct AddFriendBuilder;

struct AcceptFriend;
struct AcceptFriendBuilder;

struct PlayState;
struct PlayStateBuilder;

struct CreateUser;
struct CreateUserBuilder;

struct UserState FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UserStateBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_STATE = 4,
    VT_ID = 6
  };
  uint8_t state() const {
    return GetField<uint8_t>(VT_STATE, 0);
  }
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_STATE) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           verifier.EndTable();
  }
};

struct UserStateBuilder {
  typedef UserState Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_state(uint8_t state) {
    fbb_.AddElement<uint8_t>(UserState::VT_STATE, state, 0);
  }
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(UserState::VT_ID, id);
  }
  explicit UserStateBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<UserState> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UserState>(end);
    return o;
  }
};

inline flatbuffers::Offset<UserState> CreateUserState(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t state = 0,
    flatbuffers::Offset<flatbuffers::String> id = 0) {
  UserStateBuilder builder_(_fbb);
  builder_.add_id(id);
  builder_.add_state(state);
  return builder_.Finish();
}

inline flatbuffers::Offset<UserState> CreateUserStateDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t state = 0,
    const char *id = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  return Eater::LoginLauncher::CreateUserState(
      _fbb,
      state,
      id__);
}

struct LoginReqData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LoginReqDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_PASSWORD = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *password() const {
    return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_PASSWORD) &&
           verifier.VerifyString(password()) &&
           verifier.EndTable();
  }
};

struct LoginReqDataBuilder {
  typedef LoginReqData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(LoginReqData::VT_ID, id);
  }
  void add_password(flatbuffers::Offset<flatbuffers::String> password) {
    fbb_.AddOffset(LoginReqData::VT_PASSWORD, password);
  }
  explicit LoginReqDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<LoginReqData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LoginReqData>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginReqData> CreateLoginReqData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> password = 0) {
  LoginReqDataBuilder builder_(_fbb);
  builder_.add_password(password);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<LoginReqData> CreateLoginReqDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *password = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  return Eater::LoginLauncher::CreateLoginReqData(
      _fbb,
      id__,
      password__);
}

struct LoginResData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LoginResDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4,
    VT_KEY = 6
  };
  uint8_t result() const {
    return GetField<uint8_t>(VT_RESULT, 0);
  }
  uint32_t key() const {
    return GetField<uint32_t>(VT_KEY, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_RESULT) &&
           VerifyField<uint32_t>(verifier, VT_KEY) &&
           verifier.EndTable();
  }
};

struct LoginResDataBuilder {
  typedef LoginResData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint8_t result) {
    fbb_.AddElement<uint8_t>(LoginResData::VT_RESULT, result, 0);
  }
  void add_key(uint32_t key) {
    fbb_.AddElement<uint32_t>(LoginResData::VT_KEY, key, 0);
  }
  explicit LoginResDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<LoginResData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LoginResData>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginResData> CreateLoginResData(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t result = 0,
    uint32_t key = 0) {
  LoginResDataBuilder builder_(_fbb);
  builder_.add_key(key);
  builder_.add_result(result);
  return builder_.Finish();
}

struct RealTimeData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RealTimeDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_FRIENDSTATE = 4,
    VT_FRIENDREQUEST = 6
  };
  const flatbuffers::Vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>> *friendstate() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>> *>(VT_FRIENDSTATE);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *friendrequest() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_FRIENDREQUEST);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_FRIENDSTATE) &&
           verifier.VerifyVector(friendstate()) &&
           verifier.VerifyVectorOfTables(friendstate()) &&
           VerifyOffset(verifier, VT_FRIENDREQUEST) &&
           verifier.VerifyVector(friendrequest()) &&
           verifier.VerifyVectorOfStrings(friendrequest()) &&
           verifier.EndTable();
  }
};

struct RealTimeDataBuilder {
  typedef RealTimeData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_friendstate(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>>> friendstate) {
    fbb_.AddOffset(RealTimeData::VT_FRIENDSTATE, friendstate);
  }
  void add_friendrequest(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> friendrequest) {
    fbb_.AddOffset(RealTimeData::VT_FRIENDREQUEST, friendrequest);
  }
  explicit RealTimeDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<RealTimeData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RealTimeData>(end);
    return o;
  }
};

inline flatbuffers::Offset<RealTimeData> CreateRealTimeData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>>> friendstate = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> friendrequest = 0) {
  RealTimeDataBuilder builder_(_fbb);
  builder_.add_friendrequest(friendrequest);
  builder_.add_friendstate(friendstate);
  return builder_.Finish();
}

inline flatbuffers::Offset<RealTimeData> CreateRealTimeDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>> *friendstate = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *friendrequest = nullptr) {
  auto friendstate__ = friendstate ? _fbb.CreateVector<flatbuffers::Offset<Eater::LoginLauncher::UserState>>(*friendstate) : 0;
  auto friendrequest__ = friendrequest ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*friendrequest) : 0;
  return Eater::LoginLauncher::CreateRealTimeData(
      _fbb,
      friendstate__,
      friendrequest__);
}

struct AddFriend FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AddFriendBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_NAME = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
};

struct AddFriendBuilder {
  typedef AddFriend Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(AddFriend::VT_ID, id);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(AddFriend::VT_NAME, name);
  }
  explicit AddFriendBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<AddFriend> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AddFriend>(end);
    return o;
  }
};

inline flatbuffers::Offset<AddFriend> CreateAddFriend(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0) {
  AddFriendBuilder builder_(_fbb);
  builder_.add_name(name);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<AddFriend> CreateAddFriendDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *name = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return Eater::LoginLauncher::CreateAddFriend(
      _fbb,
      id__,
      name__);
}

struct AcceptFriend FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AcceptFriendBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_NAME = 6,
    VT_ISACCEPT = 8
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool isaccept() const {
    return GetField<uint8_t>(VT_ISACCEPT, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint8_t>(verifier, VT_ISACCEPT) &&
           verifier.EndTable();
  }
};

struct AcceptFriendBuilder {
  typedef AcceptFriend Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(AcceptFriend::VT_ID, id);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(AcceptFriend::VT_NAME, name);
  }
  void add_isaccept(bool isaccept) {
    fbb_.AddElement<uint8_t>(AcceptFriend::VT_ISACCEPT, static_cast<uint8_t>(isaccept), 0);
  }
  explicit AcceptFriendBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<AcceptFriend> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AcceptFriend>(end);
    return o;
  }
};

inline flatbuffers::Offset<AcceptFriend> CreateAcceptFriend(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    bool isaccept = false) {
  AcceptFriendBuilder builder_(_fbb);
  builder_.add_name(name);
  builder_.add_id(id);
  builder_.add_isaccept(isaccept);
  return builder_.Finish();
}

inline flatbuffers::Offset<AcceptFriend> CreateAcceptFriendDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *name = nullptr,
    bool isaccept = false) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return Eater::LoginLauncher::CreateAcceptFriend(
      _fbb,
      id__,
      name__,
      isaccept);
}

struct PlayState FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef PlayStateBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_STATE = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  bool state() const {
    return GetField<uint8_t>(VT_STATE, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyField<uint8_t>(verifier, VT_STATE) &&
           verifier.EndTable();
  }
};

struct PlayStateBuilder {
  typedef PlayState Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(PlayState::VT_ID, id);
  }
  void add_state(bool state) {
    fbb_.AddElement<uint8_t>(PlayState::VT_STATE, static_cast<uint8_t>(state), 0);
  }
  explicit PlayStateBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<PlayState> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<PlayState>(end);
    return o;
  }
};

inline flatbuffers::Offset<PlayState> CreatePlayState(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    bool state = false) {
  PlayStateBuilder builder_(_fbb);
  builder_.add_id(id);
  builder_.add_state(state);
  return builder_.Finish();
}

inline flatbuffers::Offset<PlayState> CreatePlayStateDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    bool state = false) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  return Eater::LoginLauncher::CreatePlayState(
      _fbb,
      id__,
      state);
}

struct CreateUser FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CreateUserBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_PASSWORD = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *password() const {
    return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_PASSWORD) &&
           verifier.VerifyString(password()) &&
           verifier.EndTable();
  }
};

struct CreateUserBuilder {
  typedef CreateUser Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(CreateUser::VT_ID, id);
  }
  void add_password(flatbuffers::Offset<flatbuffers::String> password) {
    fbb_.AddOffset(CreateUser::VT_PASSWORD, password);
  }
  explicit CreateUserBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CreateUser> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CreateUser>(end);
    return o;
  }
};

inline flatbuffers::Offset<CreateUser> CreateCreateUser(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> password = 0) {
  CreateUserBuilder builder_(_fbb);
  builder_.add_password(password);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<CreateUser> CreateCreateUserDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *password = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  return Eater::LoginLauncher::CreateCreateUser(
      _fbb,
      id__,
      password__);
}

}  // namespace LoginLauncher
}  // namespace Eater

#endif  // FLATBUFFERS_GENERATED_LOGINLAUNCHER_EATER_LOGINLAUNCHER_H_
