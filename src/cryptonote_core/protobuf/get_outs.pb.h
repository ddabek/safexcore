// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: get_outs.proto

#ifndef PROTOBUF_get_5fouts_2eproto__INCLUDED
#define PROTOBUF_get_5fouts_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace safex {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_get_5fouts_2eproto();
void protobuf_AssignDesc_get_5fouts_2eproto();
void protobuf_ShutdownFile_get_5fouts_2eproto();

class Out_entry;
class Outs;

// ===================================================================

class Out_entry : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:safex.Out_entry) */ {
 public:
  Out_entry();
  virtual ~Out_entry();

  Out_entry(const Out_entry& from);

  inline Out_entry& operator=(const Out_entry& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Out_entry& default_instance();

  void Swap(Out_entry* other);

  // implements Message ----------------------------------------------

  inline Out_entry* New() const { return New(NULL); }

  Out_entry* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Out_entry& from);
  void MergeFrom(const Out_entry& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Out_entry* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bytes key = 1;
  void clear_key();
  static const int kKeyFieldNumber = 1;
  const ::std::string& key() const;
  void set_key(const ::std::string& value);
  void set_key(const char* value);
  void set_key(const void* value, size_t size);
  ::std::string* mutable_key();
  ::std::string* release_key();
  void set_allocated_key(::std::string* key);

  // optional bool unlocked = 2;
  void clear_unlocked();
  static const int kUnlockedFieldNumber = 2;
  bool unlocked() const;
  void set_unlocked(bool value);

  // optional uint64 height = 3;
  void clear_height();
  static const int kHeightFieldNumber = 3;
  ::google::protobuf::uint64 height() const;
  void set_height(::google::protobuf::uint64 value);

  // optional bytes txid = 4;
  void clear_txid();
  static const int kTxidFieldNumber = 4;
  const ::std::string& txid() const;
  void set_txid(const ::std::string& value);
  void set_txid(const char* value);
  void set_txid(const void* value, size_t size);
  ::std::string* mutable_txid();
  ::std::string* release_txid();
  void set_allocated_txid(::std::string* txid);

  // @@protoc_insertion_point(class_scope:safex.Out_entry)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr key_;
  ::google::protobuf::uint64 height_;
  ::google::protobuf::internal::ArenaStringPtr txid_;
  bool unlocked_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_get_5fouts_2eproto();
  friend void protobuf_AssignDesc_get_5fouts_2eproto();
  friend void protobuf_ShutdownFile_get_5fouts_2eproto();

  void InitAsDefaultInstance();
  static Out_entry* default_instance_;
};
// -------------------------------------------------------------------

class Outs : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:safex.Outs) */ {
 public:
  Outs();
  virtual ~Outs();

  Outs(const Outs& from);

  inline Outs& operator=(const Outs& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Outs& default_instance();

  void Swap(Outs* other);

  // implements Message ----------------------------------------------

  inline Outs* New() const { return New(NULL); }

  Outs* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Outs& from);
  void MergeFrom(const Outs& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Outs* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .safex.Out_entry outs = 1;
  int outs_size() const;
  void clear_outs();
  static const int kOutsFieldNumber = 1;
  const ::safex::Out_entry& outs(int index) const;
  ::safex::Out_entry* mutable_outs(int index);
  ::safex::Out_entry* add_outs();
  ::google::protobuf::RepeatedPtrField< ::safex::Out_entry >*
      mutable_outs();
  const ::google::protobuf::RepeatedPtrField< ::safex::Out_entry >&
      outs() const;

  // optional string status = 2;
  void clear_status();
  static const int kStatusFieldNumber = 2;
  const ::std::string& status() const;
  void set_status(const ::std::string& value);
  void set_status(const char* value);
  void set_status(const char* value, size_t size);
  ::std::string* mutable_status();
  ::std::string* release_status();
  void set_allocated_status(::std::string* status);

  // @@protoc_insertion_point(class_scope:safex.Outs)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::safex::Out_entry > outs_;
  ::google::protobuf::internal::ArenaStringPtr status_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_get_5fouts_2eproto();
  friend void protobuf_AssignDesc_get_5fouts_2eproto();
  friend void protobuf_ShutdownFile_get_5fouts_2eproto();

  void InitAsDefaultInstance();
  static Outs* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Out_entry

// optional bytes key = 1;
inline void Out_entry::clear_key() {
  key_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Out_entry::key() const {
  // @@protoc_insertion_point(field_get:safex.Out_entry.key)
  return key_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Out_entry::set_key(const ::std::string& value) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:safex.Out_entry.key)
}
inline void Out_entry::set_key(const char* value) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:safex.Out_entry.key)
}
inline void Out_entry::set_key(const void* value, size_t size) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:safex.Out_entry.key)
}
inline ::std::string* Out_entry::mutable_key() {
  
  // @@protoc_insertion_point(field_mutable:safex.Out_entry.key)
  return key_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Out_entry::release_key() {
  // @@protoc_insertion_point(field_release:safex.Out_entry.key)
  
  return key_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Out_entry::set_allocated_key(::std::string* key) {
  if (key != NULL) {
    
  } else {
    
  }
  key_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), key);
  // @@protoc_insertion_point(field_set_allocated:safex.Out_entry.key)
}

// optional bool unlocked = 2;
inline void Out_entry::clear_unlocked() {
  unlocked_ = false;
}
inline bool Out_entry::unlocked() const {
  // @@protoc_insertion_point(field_get:safex.Out_entry.unlocked)
  return unlocked_;
}
inline void Out_entry::set_unlocked(bool value) {
  
  unlocked_ = value;
  // @@protoc_insertion_point(field_set:safex.Out_entry.unlocked)
}

// optional uint64 height = 3;
inline void Out_entry::clear_height() {
  height_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Out_entry::height() const {
  // @@protoc_insertion_point(field_get:safex.Out_entry.height)
  return height_;
}
inline void Out_entry::set_height(::google::protobuf::uint64 value) {
  
  height_ = value;
  // @@protoc_insertion_point(field_set:safex.Out_entry.height)
}

// optional bytes txid = 4;
inline void Out_entry::clear_txid() {
  txid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Out_entry::txid() const {
  // @@protoc_insertion_point(field_get:safex.Out_entry.txid)
  return txid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Out_entry::set_txid(const ::std::string& value) {
  
  txid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:safex.Out_entry.txid)
}
inline void Out_entry::set_txid(const char* value) {
  
  txid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:safex.Out_entry.txid)
}
inline void Out_entry::set_txid(const void* value, size_t size) {
  
  txid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:safex.Out_entry.txid)
}
inline ::std::string* Out_entry::mutable_txid() {
  
  // @@protoc_insertion_point(field_mutable:safex.Out_entry.txid)
  return txid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Out_entry::release_txid() {
  // @@protoc_insertion_point(field_release:safex.Out_entry.txid)
  
  return txid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Out_entry::set_allocated_txid(::std::string* txid) {
  if (txid != NULL) {
    
  } else {
    
  }
  txid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), txid);
  // @@protoc_insertion_point(field_set_allocated:safex.Out_entry.txid)
}

// -------------------------------------------------------------------

// Outs

// repeated .safex.Out_entry outs = 1;
inline int Outs::outs_size() const {
  return outs_.size();
}
inline void Outs::clear_outs() {
  outs_.Clear();
}
inline const ::safex::Out_entry& Outs::outs(int index) const {
  // @@protoc_insertion_point(field_get:safex.Outs.outs)
  return outs_.Get(index);
}
inline ::safex::Out_entry* Outs::mutable_outs(int index) {
  // @@protoc_insertion_point(field_mutable:safex.Outs.outs)
  return outs_.Mutable(index);
}
inline ::safex::Out_entry* Outs::add_outs() {
  // @@protoc_insertion_point(field_add:safex.Outs.outs)
  return outs_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::safex::Out_entry >*
Outs::mutable_outs() {
  // @@protoc_insertion_point(field_mutable_list:safex.Outs.outs)
  return &outs_;
}
inline const ::google::protobuf::RepeatedPtrField< ::safex::Out_entry >&
Outs::outs() const {
  // @@protoc_insertion_point(field_list:safex.Outs.outs)
  return outs_;
}

// optional string status = 2;
inline void Outs::clear_status() {
  status_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Outs::status() const {
  // @@protoc_insertion_point(field_get:safex.Outs.status)
  return status_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Outs::set_status(const ::std::string& value) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:safex.Outs.status)
}
inline void Outs::set_status(const char* value) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:safex.Outs.status)
}
inline void Outs::set_status(const char* value, size_t size) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:safex.Outs.status)
}
inline ::std::string* Outs::mutable_status() {
  
  // @@protoc_insertion_point(field_mutable:safex.Outs.status)
  return status_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Outs::release_status() {
  // @@protoc_insertion_point(field_release:safex.Outs.status)
  
  return status_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Outs::set_allocated_status(::std::string* status) {
  if (status != NULL) {
    
  } else {
    
  }
  status_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), status);
  // @@protoc_insertion_point(field_set_allocated:safex.Outs.status)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace safex

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_get_5fouts_2eproto__INCLUDED